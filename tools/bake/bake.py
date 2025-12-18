"""
Description:
    This tool implements a simple build interface wrapper for cmake.
"""

import argparse
import logging
import sys
import subprocess

from pathlib import Path

lg = logging.getLogger(Path(__file__).stem)

def build():
    """
    Docstring for build TODO: DOCS
    """

    subprocess.run("cmake", "--build", "build")

def test():
    """
    Docstring for test TODO: DOCS
    """

def main():
    """
    Description:
        Entry point for the script.
        
        parses system arguments, running operations conditionally.
    """

    parser = argparse.ArgumentParser(prog=Path(__file__).stem, description=__doc__)
    group = parser.add_argument_group("run options", "Possible ways to run the script")
    group.add_mutually_exclusive_group(required=True)

    group.add_argument(
        "-b", "--build", help="Wrapper for the equivalent cmake function"
    )

    group.add_argument(
        "-t", "--test", help="Builds the test suite, and runs all tests in a series"
    )

    args = parser.parse_args()
    if args.build:
        res = build()
    elif args.test:
        res = test()

    return res

if __name__ == "__main__":
    logging.basicConfig()
    lg.setLevel(logging.CRITICAL)

    try: sys.exit(main())
    except KeyboardInterrupt:
        lg.info(f"Exiting {Path(__file__).stem}...")
    except RuntimeError as e:
        lg.error(e)
        lg.debug("Failed to handle error. Aborting...")
