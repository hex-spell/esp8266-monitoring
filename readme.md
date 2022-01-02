# esp8266-monitoring
Async websockets server that sends sensor data to all clients

# Current sensors
 - Flame (HW-491 / KY-026)
 - Temperature (HW-506 / Ds18b20 / KY-001)

# TODO
- Add WebSockets client and connect to a f1-micro gpc vps instance that relays socket events its clients (to make socket events available on internet instead of localhost).
- Add humidity, light, tds, water temperature, ph and movement sensors
- Create android app that responds to socket events with photos
- Send a "take photo" socket event everyday at the same hour