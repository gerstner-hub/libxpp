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
    env = initSCons("libX++")

if not env.ExistsLib("libcosmos"):
    cosmos_env = env.Clone()
    cosmos_env['buildroot'] = ""
    SConscript('libcosmos/SConstruct', duplicate=0, variant_dir=env['buildroot'] + "libcosmos/", exports={"env": cosmos_env})

env = SConscript(env['buildroot'] + 'src/SConstruct')

instroot = env['instroot']

install_dev_files = env['install_dev_files']

if env['project'] == "libX++":
    SConscript(env['buildroot'] + 'test/SConstruct')
    SConscript(env['buildroot'] + 'doc/SConstruct')
    Default(env['libs']['libX++'])

if install_dev_files or env['libtype'] == "shared":
    node = env.InstallVersionedLib(os.path.join(instroot, env['lib_base_dir']), env["libs"]["libX++"])
    env.Alias("install", node)

if install_dev_files:
    env.InstallHeaders("X++")
    node = env.Install(Path(instroot) / env['pkg_config_dir'], "data/libX++.pc")
    env.Alias("install", node)

