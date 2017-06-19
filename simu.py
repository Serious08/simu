#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys,os
import logging
import automate

def main():
  root = logging.getLogger('simu')
  root.setLevel(logging.DEBUG)

  ch = logging.StreamHandler(sys.stdout)
  ch.setLevel(logging.DEBUG)
  formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
  ch.setFormatter(formatter)
  root.addHandler(ch)

  root.info('Start ip simulator')

  automate = automate.Automate('ob_nominal.scr', 'bb_nominal.scr')
  automate.start()

if __name__ == "__main__":
  sys.exit(main())

