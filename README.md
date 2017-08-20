## hue_arduino_communication

Reference Links:
https://www.developers.meethue.com/documentation/getting-started

### Upon entry
url: /api
{"devicetype":"my_hue_app#android ebenezer"}

method: POST

### Authenticated Hue ID: (ID token changes each reset)
qtiZQ6RLayFCNVdwWsjxch33N4XqoPjZw422DWba

### Get a listing of all lights
url: http://192.168.1.6/api/qtiZQ6RLayFCNVdwWsjxch33N4XqoPjZw422DWba/lights

method: GET

### Call a specific Light
url: http://192.168.1.6/api/qtiZQ6RLayFCNVdwWsjxch33N4XqoPjZw422DWba/lights/[light_number]

method: GET

### Turn Light on/off
url: http://192.168.1.6/api/qtiZQ6RLayFCNVdwWsjxch33N4XqoPjZw422DWba/lights/[light_number]/state

body: {"on":[false or true]]}

method: put

### Change Hue Light Saturation and Brightness (max for sat & bri is 254, measure ofhue is from 0 to 65535)
url: http://192.168.1.6/api/qtiZQ6RLayFCNVdwWsjxch33N4XqoPjZw422DWba/lights/[light_number]/state

body: {"on":true, "sat":254, "bri":254,"hue":10000}

method: put
