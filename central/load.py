"""Load the central image using the repository's Nordic loader."""
import argparse
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parent

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--hex', type=Path, default=ROOT/'build/zephyr/zephyr.hex')
    parser.add_argument('--probe', required=True)
    parser.add_argument('--environments', type=Path)
    parser.add_argument('--dry-run', action='store_true')
    parser.add_argument('--recover', action='store_true', help='Erase the device and remove access protection before programming')
    args = parser.parse_args()
    command = [sys.executable, str(ROOT.parent/'peripheral/load.py'), 'nrf-52832',
               '--hex', str(args.hex), '--probe', args.probe]
    if args.environments is not None:
        command += ['--environments', str(args.environments)]
    if args.dry_run:
        command.append('--dry-run')
    if args.recover:
        command.append('--recover')
    return subprocess.call(command)

if __name__ == '__main__':
    sys.exit(main())
