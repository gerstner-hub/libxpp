import os
import sys
from pathlib import Path

try:
    # if there is already an environment then simply use that, some other
    # level of the build system already initialized it
    Import('env')
except Exception:
    try:
        from buildsystem import initSCons
    except ImportError:
        cosmos_scripts = Path(Dir('.').abspath) / "libcosmos" / "scripts"
        sys.path.append(str(cosmos_scripts))
        from buildsystem import initSCons
    env = initSCons("libxpp")

env.AddLocalLibrary("libcosmos")

env = SConscript(env['buildroot'] + 'src/SConstruct')

instroot = env['instroot']

install_dev_files = env['install_dev_files']

if env['project'] == "libxpp":
    SConscript(env['buildroot'] + 'test/SConstruct')
    SConscript(env['buildroot'] + 'doc/SConstruct')
    Default(env['libs']['libxpp'])

if install_dev_files or env['libtype'] == "shared":
    node = env.InstallVersionedLib(os.path.join(instroot, env['lib_base_dir']), env["libs"]["libxpp"])
    env.Alias("install", node)

if install_dev_files:
    env.InstallHeaders("xpp")
    node = env.Install(Path(instroot) / env['pkg_config_dir'], "data/libxpp.pc")
    env.Alias("install", node)
