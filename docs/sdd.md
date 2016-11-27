# obsgetter Design Document

## General Overview
_obsgetter_ is a command line application for interfacing with [Government Weather site](http://w1.weather.gov). It takes a two digit state code from the user, presents them with a list of available weather stations, take their selection, and then show them the weather report for the chosen station. The output is displayed according to weather.cfg. This configuration follows the standard "title"="key" pattern, with the "#" sign representing a comment. Here is the default configuration file. 

```
# Format is title=key
Station=station_id
Location=location
Weather=weather
Wind Data=wind_string
Temp=temperature_string
Relative Humidity=relative_humidity
```

_obsgetter_ will find the value represented by the given key within the weather report. It will then display the found value with the title provided in the configuration file.

### Complete Requirements list
Due to the small scope of this project, the software requirements will reside here within the design document.

---
* SRD-1 The obsgetter application shall display a list of available station identifiers
* SRD-2 The obsgetter application shall prompt a user to enter a station identifier
* SRD-3 The obsgetter application shall set a timer for 60 seconds.
* SRD-4 If the user does not enter a URL before the timer expires, a signal handler shall terminate the obsgetter application.
* SRD-5 The obsgetter application shall read a station identifier entered by the user
* SRD-6 The obsgetter application shall verify that the station is valid according to its list of stations.
* SRD-7 Upon receiving a valid station id, the obsgetter application shall cancel the timer.
* SRD-8 The obsgetter application shall create a TCP session to the weather server on port 80
* SRD-9 The obsgetter application shall use the HTTP POST command to retrieve the page and store it in a file on the local machine.
* SRD-10 The obsgetter application shall include a Host header with the value ‘w1.weather.gov’
* SRD-11 The obsgetter application shall include a Connection header with the value ‘close’
* SRD-12 The obsgetter application shall include a User-Agent header with the value ‘obsgetter/1.0’ with the POST command
* SRD-13 The obsgetter application shall include an Accept header with the value ‘*/*’
* SRD-14 The obsgetter application shall parse the returned xml to extract strings of observation data
* SRD-15 The obsgetter application shall display the station_id
* SRD-16 The obsgetter application shall display the location
* SRD-17 The obsgetter application shall display the weather string
* SRD-18 The obsgetter application shall display the wind string
* SRD-19 The obsgetter application shall display the temperature string
* SRD-20 The obsgetter application shall display the relative humidity
* SRD-21 The obsgetter application shall, when finished displaying data, prompt the user for another station identifier.

---

### Fully Compliant Requirements

---
* SRD-1
* SRD-2
* SRD-3
* SRD-4
* SRD-5
* SRD-6
* SRD-7
* SRD-12
* SRD-14
* SRD-21

---

### Partially Compliant Requirements

---
* SRD-8
	* **Justification:** The C library [libcurl](https://curl.haxx.se/libcurl/) is used for the HTTP interface. This removes the need to set the majority of the HTTP headers.
* SRD-9
	* **Justification:** File is only kept in memory, never stored to disk.
* SRD-10
	* **Justification:** The C library [libcurl](https://curl.haxx.se/libcurl/) is used for the HTTP interface. This removes the need to set the majority of the HTTP headers.
* SRD-11
	* **Justification:** The C library [libcurl](https://curl.haxx.se/libcurl/) is used for the HTTP interface. This removes the need to set the majority of the HTTP headers.
* SRD-13
	* **Justification:** The C library [libcurl](https://curl.haxx.se/libcurl/) is used for the HTTP interface. This removes the need to set the majority of the HTTP headers.
* SRD-15
	* **Justification:** The _obsgetter_ application reads the fields to be parsed & displayed from a configuration file. The default configuration will be the fields required in the supplied requirements.
* SRD-16
	* **Justification:** The _obsgetter_ application reads the fields to be parsed & displayed from a configuration file. The default configuration will be the fields required in the supplied requirements.ß
* SRD-17
	* **Justification:** The _obsgetter_ application reads the fields to be parsed & displayed from a configuration file. The default configuration will be the fields required in the supplied requirements.
* SRD-18
	* **Justification:** The _obsgetter_ application reads the fields to be parsed & displayed from a configuration file. The default configuration will be the fields required in the supplied requirements.
* SRD-19
	* **Justification:** The _obsgetter_ application reads the fields to be parsed & displayed from a configuration file. The default configuration will be the fields required in the supplied requirements.
* SRD-20
	* **Justification:** The _obsgetter_ application reads the fields to be parsed & displayed from a configuration file. The default configuration will be the fields required in the supplied requirements.

---

## Architecture
The _obsgetter_ application is broken up into four primary modules and the main loop. There is module for XML related operations, a module for IO, a module for HTTP interfaces, and a module for custom types. The main loop utilizes each of these modules to implement the software requirements listed above. See the detailed design section for more information on each module.

## Detailed Design

---
#### CString

* void create_cstr(struct cstr *string);
* void destroy_cstr(struct cstr *string);
* void convert_to_upper(char* string);

The CString module provides access to the type _struct cstr_ and the function calls. This provides single structure that can be used to track a char pointer and the size of the string stored at the location. It allows for simple abstractions to pass the string between the various modules. It provides an easy way to allocate/deallocate the string, as well as just a single parameter to pass between the function.

---
#### HTTP

* void http_get(char *url, struct cstr *string);

The HTTP module provides a wrapper around the libcurl library. It provides a single function with the following footprint.
This function takes a url in the format of a char pointer, retrieves the web page stored there, and saves it in the cstr structure.

---
#### SAFE_IO

* int get_line_safe(char *prompt, char *out_buffer, size_t size);
* int get_timed_line(char *prompt, char *out_buffer, size_t size, int seconds);

The SAFE_IO module provides methods to simplify getting commandline input in a safe/clean manner. It also provides an interface to retrieve input from the user with an attached timer. These interfaces ensure that the input data; fits in the supplied buffer, a prompt is printed if needed, that the buffer is properly terminated when finished, and that STDIN is flushed so we don't corrupt the next input.

---
#### XML_UTILS

* void print_xml_value(char *xml, char* key);
* void pretty_print_stations(char *xml, char* state);

The XML_UTILS module provides two methods that are used by the main loop. The first is a generic interfaces that takes a XML string and a key. The function finds the key and prints the value stored in that node. The second is a interface specific to _obsgetter_. It takes a two character state code and returns every weather station that is associated with that state code. It expects the provided XML to be formated like the following snippet.

```
<wx_station_index>
<credit>NOAA's National Weather Service</credit>
<credit_URL>http://weather.gov/</credit_URL>
<image>
	<url>http://weather.gov/images/xml_logo.gif</url>
	<title>NOAA's National Weather Service</title>
	<link>http://weather.gov</link>
</image>
<suggested_pickup>08:00 EST</suggested_pickup>
<suggested_pickup_period>1140</suggested_pickup_period>
<station>
	<station_id>CWAV</station_id>
	<state>AB</state>
	<station_name>Sundre</station_name>
	<latitude>51.76667</latitude>
	<longitude>-114.68333</longitude>
	<html_url>http://weather.noaa.gov/weather/current/CWAV.html</html_url>
	<rss_url>http://weather.gov/xml/current_obs/CWAV.rss</rss_url>
	<xml_url>http://weather.gov/xml/current_obs/CWAV.xml</xml_url>
</station>
</wx_station_index>
```

---
#### MAIN
The main loop is the core business logic of the _obsgetter_ application. It contains the prompts for user input, the configuration reading logic, the display logic, and the signal handler needed to catch the signal generated if the user input timeouts. It utilizes all of the other modules listed above to implement the core requirement set.


