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
	* **Justification:** The C library [lib curl](https://curl.haxx.se/libcurl/) is used for the HTTP interface. This removes the need to set the majority of the HTTP headers.
* SRD-9
	* **Justification:** File is only kept in memory, never stored to disk.
* SRD-10
	* **Justification:** The C library [lib curl](https://curl.haxx.se/libcurl/) is used for the HTTP interface. This removes the need to set the majority of the HTTP headers.
* SRD-11
	* **Justification:** The C library [lib curl](https://curl.haxx.se/libcurl/) is used for the HTTP interface. This removes the need to set the majority of the HTTP headers.
* SRD-13
	* **Justification:** The C library [lib curl](https://curl.haxx.se/libcurl/) is used for the HTTP interface. This removes the need to set the majority of the HTTP headers.
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
*Describe how the application is broken into modules that handle
different types of functionality. Each ‘module’ will be placed in a
separate source file making the code more maintainable.*

## Detailed Design

*This section should describe the external API for each module in
the application and data structures in each module.*
