import os
import lit

config.name = "Unit Test"
config.test_format = lit.formats.ShTest(True)
config.suffixes = ['.mlir']
config.test_source_root = os.path.dirname(__file__)
config.test_exec_root = os.path.join(config.test_source_root)
config.excludes = ['lit.cfg.py']

project_dir = os.path.dirname(config.test_source_root)
llvm_dir = os.getenv('LLVM_BUILD_DIR') \
    if os.getenv('LLVM_BUILD_DIR', '') != '' else f"{project_dir}/third_party/llvm-project/build"

config.environment['PATH'] = (
    ":".join([f"{project_dir}/bin",
              f"{llvm_dir}/bin",
              os.environ['PATH']])
)