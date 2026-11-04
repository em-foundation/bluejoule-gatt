"""Build a BlueJoule platform using an externally prepared environment (Python 3.10+)."""
import argparse
import json
import os
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parent

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--environments', type=Path, default=ROOT.parent/'peripheral/environments.local.json')
    parser.add_argument('--dry-run', action='store_true')
    args = parser.parse_args()
    try:
        settings = json.loads(args.environments.read_text(encoding='utf-8-sig'))['build']['nordic']
        paths = {k: Path(settings[k]).expanduser().resolve() for k in ('python','zephyr','sdk')}
        for key, path in paths.items():
            if not path.exists():
                raise ValueError(f'{key} does not exist: {path}')
        env = os.environ.copy()
        env.update(ZEPHYR_BASE=str(paths['zephyr']), ZEPHYR_SDK_INSTALL_DIR=str(paths['sdk']), ZEPHYR_TOOLCHAIN_VARIANT='zephyr', PYTHONDONTWRITEBYTECODE='1')
        env.pop('PYTHONHOME', None)
        env.pop('PYTHONPATH', None)
        env['PATH'] = str(paths['python'].parent) + os.pathsep + env.get('PATH','')
        if 'toolchain' in settings:
            toolchain = Path(settings['toolchain']).expanduser().resolve()
            env['PATH'] = os.pathsep.join(str(toolchain/p) for p in ('bin','opt/bin','opt/bin/Scripts')) + os.pathsep + env['PATH']
        command = [str(paths['python']), '-m', 'west', 'build', '--no-sysbuild', '-p', 'always',
                   '-b', 'nrf52dk/nrf52832', '-d', str(ROOT/'build'), str(ROOT), '--', '-DCONFIG_DEBUG_THREAD_INFO=y']
        print('Command:', json.dumps(command), flush=True)
        if not args.dry_run:
            return subprocess.call(command, cwd=ROOT, env=env)
        return 0
    except (OSError, ValueError, KeyError) as exc:
        parser.exit(2, f'Environment setup required: {exc}. See README.md and environments.example.json.\n')

if __name__ == '__main__':
    sys.exit(main())
