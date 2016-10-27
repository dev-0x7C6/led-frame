# Protocol
## Notifications

### Messages
* **notification** - notification about something that happend
* **version** - version of protocol

*content in header:*
```javascript
{
	"message": "notification" // string
	"version": 1 // int
}
```

### Events
* **attached** - interface added
* **detached** - interface removed 
* **modified** - interface modified

*content in header:*
```javascript
{
	"event": "attached" // string
}
```

### Sources
* **emitter** - source of led data, it could be color, animation, wallpaper or display output
* **receiver** - receiver of led data, mainly uart device
* **corrector** - led data corrector, it used to tune up led data from emitter before receiver show it

*content in header:*
```javascript
{
	"source": "receiver" // string
}
```

### Content
#### Emitter data
*content in header:*
```javascript
{
	"datagram": {
		"id": int, // unique id
		"type": string, // type (animation, color, display and etc.)
		"name": string, // user nickname
		"description": string, // details and etc.
	}
}
```

#### Receiver data
*content in header:*
```javascript
{
	"datagram": {
		"id": int, // unique id (of receiver aka device)
		"type": string, // type (uart device)
		"name": string, // user nickname
		"emitter": int, // id of emitter that is connected to receiver
	}
}
```

#### Corrector data
*content in header:*
```javascript
{
	"datagram": {
		"id": int, // unique id
		"type": string, // type (brightness, rgb correction and etc.)
		"owner": int, // id of receiver that is using corrector
		"factor": double, // value of correction
		"min":double, // minimal value
		"max": double, // maximum value
	}
}
```

### Example
*complete json header that notify that "corrector" is being attached:*
```javascript
{
	"message": "notification",
	"version": 1,
	"event": "attached"
	"source": "corrector"

	"datagram": {
		"id": 15,
		"type": "brightness",
		"owner": 3,
		"factor": 3.0,
		"min": 1.0,
		"max": 6.0,
	}
}
```
