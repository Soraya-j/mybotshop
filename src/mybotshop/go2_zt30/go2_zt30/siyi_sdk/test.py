from siyi_sdk import SIYISDK
# from go2_zt30.siyi_sdk.siyi_sdk import SIYISDK


siyi = SIYISDK(debug=True)

# Get firmware version
msg = siyi.firmwareVerMsg()
print("Firmware version command:", msg)

# Zoom In
msg = siyi.zoomInMsg()
print("Zoom In command:", msg)

# Absolute Zoom (e.g. 10.5x)
msg = siyi.absoluteZoomMsg(10.5)
print("Absolute Zoom command:", msg)
