# 2026-07-06T16:20:15.825489900
import vitis

client = vitis.create_client()
client.set_workspace(path="workspac1")

platform = client.create_platform_component(name = "platform",hw_design = "$COMPONENT_LOCATION/../../Zybo-Z7-20-HDMI-v2018.2-2/design_1_wrapper.xsa",os = "standalone",cpu = "ps7_cortexa9_0",domain_name = "standalone_ps7_cortexa9_0",compiler = "gcc")

