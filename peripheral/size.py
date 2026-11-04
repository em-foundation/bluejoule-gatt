"""Show an existing firmware's size, or Zephyr ROM/RAM symbol reports."""
import argparse
import json
import os
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parent

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('platform', type=lambda v: v.replace(chr(92), '/').rstrip('/').rsplit('/', 1)[-1], choices=sorted(p.name for p in (ROOT/'platforms').iterdir() if p.is_dir()))
    parser.add_argument('--report', choices=['rom','ram','all'], help='Zephyr symbol/source breakdown; requires the prepared build environment')
    parser.add_argument('--environments', type=Path, default=ROOT/'environments.local.json')
    args = parser.parse_args()
    try:
        platform = json.loads((ROOT/'platforms'/args.platform/'platform.json').read_text())
        settings = json.loads(args.environments.read_text())['build'][platform['environment']]
        directory = Path(settings.get('build_root', ROOT/'build')).expanduser().resolve()/args.platform
        if platform.get('sysbuild'):
            directory /= 'peripheral'
        cache = directory/'CMakeCache.txt'
        if not cache.is_file():
            raise ValueError('No configured build found; run build.py for this platform first')
        values = {}
        for line in cache.read_text().splitlines():
            if '=' in line and ':' in line and not line.startswith(('#','//')):
                values[line.split(':',1)[0]] = line.split('=',1)[1]
        elf = directory/'zephyr/zephyr.elf'
        if not elf.is_file():
            raise ValueError('No firmware ELF found; complete the build first')
        tool = values.get('CMAKE_SIZE')
        if not tool:
            compiler = Path(values['CMAKE_C_COMPILER'])
            tool = str(compiler.with_name(compiler.name.replace('gcc','size')))
        print('Platform:', args.platform, '\nELF:', elf, flush=True)
        result = subprocess.call([tool, str(elf)])
        if result or not args.report:
            return result
        env = os.environ.copy()
        env['BLUEJOULE_PLATFORM'] = args.platform
        python = Path(settings['python']).expanduser().resolve()
        env['PATH'] = str(python.parent)+os.pathsep+env.get('PATH','')
        if 'toolchain' in settings:
            tc = Path(settings['toolchain'])
            env['PATH'] = os.pathsep.join(str(tc/p) for p in ('bin','opt/bin','opt/bin/Scripts'))+os.pathsep+env['PATH']
        env['ZEPHYR_BASE'] = settings['zephyr']
        env.pop('PYTHONHOME',None)
        env.pop('PYTHONPATH',None)
        reports = ['rom','ram'] if args.report=='all' else [args.report]
        for report in reports:
            result = subprocess.call([values['CMAKE_COMMAND'],'--build',str(directory),'--target',report+'_report'],env=env)
            if result:
                return result
        return 0
    except (OSError, ValueError, KeyError) as exc:
        parser.exit(2, f'Size report unavailable: {exc}. See README.md.\n')

if __name__ == '__main__':
    sys.exit(main())
