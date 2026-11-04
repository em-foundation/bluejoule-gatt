"""Load a board-compatible HEX using native vendor tools (Python 3.10+)."""
import argparse
import json
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parent

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('platform', type=lambda value: value.replace(chr(92), '/').rstrip('/').rsplit('/', 1)[-1], choices=sorted(p.name for p in (ROOT/'platforms').iterdir() if p.is_dir()))
    parser.add_argument('--hex', type=Path, dest='image')
    parser.add_argument('--probe', help='Probe serial number')
    parser.add_argument('--environments', type=Path, default=ROOT/'environments.local.json')
    parser.add_argument('--dry-run', action='store_true')
    parser.add_argument('--recover', action='store_true', help='Nordic only: erase device and remove access protection before programming')
    args = parser.parse_args()
    try:
        platform = json.loads((ROOT/'platforms'/args.platform/'platform.json').read_text())
        environments = json.loads(args.environments.read_text())
        settings = environments['load'][platform.get('load_environment', platform['environment'])]
        tool = Path(settings['executable']).expanduser().resolve()
        build_root = ROOT/'build'
        if args.image is None:
            build_root = Path(environments.get('build', {}).get(platform['environment'], {}).get('build_root', build_root))
        default_image = build_root/args.platform/('merged.hex' if platform.get('sysbuild') else 'zephyr/zephyr.hex')
        image = (args.image or default_image).expanduser().resolve()
        if not tool.is_file():
            raise ValueError(f'Loader executable not found: {tool}')
        if not image.is_file() or image.suffix.lower() != '.hex':
            raise ValueError(f'HEX file not found or wrong extension: {image}')
        if args.recover and platform['loader'] != 'nrfutil':
            raise ValueError('--recover is supported only for Nordic platforms')
        if platform['loader'] == 'commander':
            command = [str(tool), 'flash', '--device', platform['device']]
            if args.probe:
                command += ['--serialno', args.probe]
            command += [str(image)]
        elif platform['loader'] == 'stm32cubeprogrammer':
            command = [str(tool), '--connect', 'port=swd', 'reset=HWrst']
            if args.probe:
                command += ['sn='+args.probe]
            command += ['--download', str(image), '--start']
        elif platform['loader'] == 'nrfutil':
            command = [str(tool), 'device', 'program', '--firmware', str(image)]
            if args.probe:
                command += ['--serial-number', args.probe]
            else:
                raise ValueError('Nordic loading requires --probe SERIAL to select the device')
            command += ['--options', 'chip_erase_mode=ERASE_RANGES_TOUCHED_BY_FIRMWARE,verify=VERIFY_READ,reset=RESET_SYSTEM']
        elif platform['loader'] == 'uniflash':
            if args.probe:
                raise ValueError('For TI select the probe in your ccxml file, not --probe')
            config = Path(settings['config']).expanduser().resolve()
            if not config.is_file():
                raise ValueError(f'Target configuration not found: {config}')
            commands = [
                [str(tool), '-c', str(config), '-s', 'VerifyAfterProgramLoad=No verification', '-f', '-t', '60', str(image)],
                [str(tool), '-c', str(config), '-s', 'ImageVerificationSetting=1', '-t', '60', str(image)],
            ]
            for command in commands:
                print('Command:', json.dumps(command), flush=True)
                if not args.dry_run:
                    result = subprocess.call(command, cwd=ROOT)
                    if result:
                        return result
            return 0
        else:
            raise ValueError('Unsupported loader: '+platform['loader'])
        if args.recover:
            recovery = [str(tool), 'device', 'recover', '--serial-number', args.probe]
            print('Recovery erases device user nonvolatile memory and removes access protection.', flush=True)
            print('Command:', json.dumps(recovery), flush=True)
            if not args.dry_run:
                result = subprocess.call(recovery, cwd=ROOT)
                if result:
                    return result
        print('Platform:', args.platform, flush=True)
        print('Command:', json.dumps(command), flush=True)
        return 0 if args.dry_run else subprocess.call(command, cwd=ROOT)
    except (OSError, ValueError, KeyError) as exc:
        parser.exit(2, f'Load setup required: {exc}. See README.md and environments.example.json.\n')

if __name__ == '__main__':
    sys.exit(main())
