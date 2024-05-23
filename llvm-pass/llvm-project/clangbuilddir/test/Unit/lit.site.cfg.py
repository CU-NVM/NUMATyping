# Autogenerated from /home/kidus/llvm-project/llvm/test/Unit/lit.site.cfg.py.in
# Do not edit!

# Allow generated file to be relocatable.
import os
import platform
def path(p):
    if not p: return ''
    # Follows lit.util.abs_path_preserve_drive, which cannot be imported here.
    if platform.system() == 'Windows':
        return os.path.abspath(os.path.join(os.path.dirname(__file__), p))
    else:
        return os.path.realpath(os.path.join(os.path.dirname(__file__), p))


import sys

config.llvm_src_root = path(r"../../../llvm")
config.llvm_obj_root = path(r"../..")
config.llvm_tools_dir = lit_config.substitute(path(r"../../bin"))
config.llvm_build_mode = lit_config.substitute(".")
config.shlibdir = lit_config.substitute(path(r"../../lib"))

# Let the main config do the real work.
lit_config.load_config(
    config, os.path.join(config.llvm_src_root, "test/Unit/lit.cfg.py"))
