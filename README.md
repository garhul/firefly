## Project abandoned, see https://github.com/garhul/aurora instead ##

# firefly

This is a somewhat boilerplate for iot devices based
on Nodemcu platform.
Provides endpoints for configuring ssid and password
and integrates easily with Swarm a centralized IoT device manager.


## Access point mode (AP)

When started up the device tries three times by default to connect to the configured
ssid. No worries, if there's no ssid configured it will fail and start in AP mode.

In this mode the device will start a network with a preconfigured SSID called "firefly"
to which you can connect and access a simple web interface for updating ssid and password.

This mode starts with an MDNS responder so you can access the configuration form by visiting
http://config.local

Endpoints for this mode are
[GET] http://config.local/ -> home which shows the config form
[GET] http://config.local/descriptor -> gets the device json descriptor file

[POST] http://config.local/setup -> sets the password and ssid for the network connection
for this mode we require the params pwd and ssid to be sent, possible responses are:
400 for an error when pwd and or ssid are empty
200 for when ssid and pwd where successfuly saved to the device eeprom

[GET] http://config.local/reboot -> which reboots the device


## Station mode (ST)
