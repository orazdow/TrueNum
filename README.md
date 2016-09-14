# TrueNumber IoT Client
<br>

**A library for arduino-compatibale boards to create data for ioT applications using TrueNumbers,**

**and for for remote configuration.**
***


**To install this library:**

-Download .zip

-extract TrueNum folder into your arduino libraries directory.
***
#### Hardware Note:
On AVR-based boards, the number of parameter numbers that can be used at once is limited to 4 to conserve memory. TrueNumbers with the properties *report-interval*, *status-report*, *return-bool* are not be retained as templates, and do not affect this limit.
***
<br>
## API:

#### Constructors
* [**TrueNum( *user, pwd, numberSpace, ID* )**](#const1)


* [**TrueNum( *user, pwd, numberSpace* )**](#const2)

***

#### Functions
* [*void* **setID( *ID* )**](#setid)


* [*void* **getQuery( *Client* )**](#gquery)


* [*void* **makeCall( *Client* )**](#makecall)


* [*void* **setVal( *token, value* )**](#setval)


* [*float* **getVal( *token* )**](#getval)


* [*boolean* **getReturnBool( )**](#rtnbool)


***
[***View example code***](#examp)
***
<br>
#### <a id="const1"></a>TrueNum( user, pwd, numberSpace, ID ) 

*Constructs a TrueNumber client*

**arguments:**

&nbsp;&nbsp;&nbsp;&nbsp; *string* **user:** &nbsp;TrueNumber username*

&nbsp;&nbsp;&nbsp;&nbsp; *string* **pwd:** &nbsp;TrueNumber password

&nbsp;&nbsp;&nbsp;&nbsp; *string* **numberSpace:** &nbsp;TrueNumber numberSpace

&nbsp;&nbsp;&nbsp;&nbsp; *string* **ID:** &nbsp;Unique board ifentifier
***

#### <a id="const2"></a>TrueNum( user, pwd, numberSpace) 

*Constructs a TrueNumber client. ID string is excluded to be set later.*

***

#### <a id="setid"></a>setID( *ID* )

*Sets the board's Identifier string*

*A TrueNumber tagged with: &nbsp;parameter:*ID* &nbsp;will be queried by the board with the getQuery() function.*



**arguments:**

&nbsp;&nbsp;&nbsp;&nbsp; *string* **ID:** &nbsp;unique identifier

**returns:**

&nbsp;&nbsp;&nbsp;&nbsp; void
***

#### <a id="gquery"></a>getQuery( *Client* )

*Queries the NumberSpace for Truenumbers to use as parameters or templates*


**arguments:**

&nbsp;&nbsp;&nbsp;&nbsp; **Client:** &nbsp;a Client object using the same interface as EthernetClient, WifiClient, yunClient, etc

**returns:**

&nbsp;&nbsp;&nbsp;&nbsp; void
***

#### <a id="makecall"></a>makeCall( *Client* )

*Generates Truenumbers using sensor data and template TrueNumbers in the NumberSpace*

*-A second TrueNumber string argument can be passed for testing, but this will be hard-coded into the sketch*



**arguments:**

&nbsp;&nbsp;&nbsp;&nbsp; **Client:** &nbsp;a Client object using the same interface as EthernetClient, WifiClient, yunClient, etc

**returns:**

&nbsp;&nbsp;&nbsp;&nbsp; void
***

#### <a id="setval"></a>setVal( *token, value* )

*Associates a token string with a sensor's output*

*-Must begin with a $ symbol.* 

*For example, $DHT-temp would be a typical token to associate with a DHT-xx Sensors temperature readings.*

    temperature of myOffice = "$DHT-temp F"
    
*would be a template truenumber to be used by the board to generate truenumbers based on a temperature sensor's readings. The associated value is updated upon each call to setVal().*


**arguments:**

&nbsp;&nbsp;&nbsp;&nbsp; *string* **token:** &nbsp;a token string to associate with a sensor's output 

&nbsp;&nbsp;&nbsp;&nbsp; *float* **value:** &nbsp;sensor output values

**returns:**

&nbsp;&nbsp;&nbsp;&nbsp; void
***

#### <a id="getval"></a>getVal( *token* )

*Returns the value currently associated with a token string*

*-useful for testing* 

**arguments:**

&nbsp;&nbsp;&nbsp;&nbsp; *string* **token:** &nbsp; a token string previously set by setVal()

**returns:**

&nbsp;&nbsp;&nbsp;&nbsp; float
***

#### <a id="rtnbool"></a>getReturnBool( )

*Returns the state of a boolean set with a "return-bool" trueNumber*

*-can be used to control code running on the board*

    return-bool of myArduino = "$DHT-temp > 30 F"

*would return true if the value associated with $DHT-temp exceeds 30 F*

**arguments:**

&nbsp;&nbsp;&nbsp;&nbsp; none

**returns:**

&nbsp;&nbsp;&nbsp;&nbsp; float
***

<br>
### <a id="examp"></a>Example sketch:
***


In this example, an Arduino yun is set up to send temperature readings from a DHT22 sensor

First construct a TrueNumber client object:

The user's credentials and NumberSpace are set, as well as the board ID.

    TrueNum trueNum("usere@truenum.com", "mypwd", "temp_readings", "board123");

and construct the client and sensor objects:

    YunClient client;
    //DHT22 on pin 12:
    DHT dht(12, DHT22); 

In setup() we will initialize the client and sensor and setup the sketch to start when the serial monitor is opened:

The TrueNumber client returns information on it's acitvity to the serial monitor .

    void setup() { 
    //initializes the yun client and sensor
    Bridge.begin(); 
    dht.begin();
    Serial.begin(9600);
    //sketch will start when the serial monitor is opened
    while(!Serial); 
    }
    
In the main loop, the TrueNumber client's 3 main functions are called:

First we set the token string to be associated with the DHT22's temperature readings:

    trueNum.setVal("$DHT-temp", mySensor.getTemperature());
    
Next, the getQuery function is called to retrieve parameter numbers from the numberSpace:

    trueNum.getQuery(client);
    
Finally, we invoke makeCall()
    
    trueNum.makeCall(client);
    
To generate new numbers in the temp_readings numberSpace using the DHT22's output.

<br>
*Here is the full sketch:*

    #include "TrueNum.h"
	#include "DHT.h" 
	#include <Bridge.h>
	#include <YunClient.h>

	TrueNum trueNum("usere@truenum.com", "mypwd", "temp_readings", "board123");

	YunClient client;
	//DHT22 on pin 12:
	DHT dht(12, DHT22);

	void setup() {
	//initializes the yun client and sensor
	Bridge.begin(); 
	dht.begin();
	Serial.begin(9600);
	//sketch will start when the serial monitor is opened
	while(!Serial); 
	}

	void loop() {
	  
	trueNum.setVal("$DHT-temp", dht.getTemperature());

	trueNum.getQuery(client);
	trueNum.makeCall(client);

	}
<br>
*Some template and parameter numbers we can use might be:*

    temperature of roomHot = $DHT-temp >= 80 F

*which will be called only if the sendor reading meets the above condition.*

    temperature of myRoomt = $DHT-temp F

*Will be called on every loop. The interval that determines the period between calls to generate TrueNumbers can be set with:*

    report-interval of myArduino = 30 s

*Which uses one of three "special" trueNumber properties. The other two being: status-report and return-bool.*

*These trueNumbers are accesible by the board by tagging them with:* board123 and a child-tag of the ID string specified in the sketch (for this example: **parameter:board123**).

*More information visit [www.truenum.com](http://www.truenum.com/)*











