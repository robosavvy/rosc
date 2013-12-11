import os
import sys

from . base import InvalidMsgSpec, EXT_MSG, MSG_DIR, SEP, log
from . names import is_legal_resource_name, is_legal_resource_base_name, package_resource_name, resource_name


def test():
    print "Test"