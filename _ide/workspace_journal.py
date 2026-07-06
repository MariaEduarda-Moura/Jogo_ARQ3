# 2026-07-06T16:21:56.511446700
import vitis

client = vitis.create_client()
client.set_workspace(path="workspac1")

comp = client.create_app_component(name="app_component",platform = "$COMPONENT_LOCATION/../platform/export/platform/platform.xpfm",domain = "standalone_ps7_cortexa9_0")

comp = client.get_component(name="app_component")
status = comp.import_files(from_loc="$COMPONENT_LOCATION/../../src-20260706T192436Z-3-001", files=["src"], dest_dir_in_cmp = "app_component", is_skip_copy_sources = False)

platform = client.get_component(name="platform")
status = platform.build()

comp = client.get_component(name="app_component")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

comp = client.get_component(name="app_component")
comp.set_app_config(key = "USER_COMPILE_OPTIMIZATION_LEVEL", values = ["-O3"])

status = platform.build()

comp = client.get_component(name="app_component")
comp.build()

status = platform.build()

comp.build()

