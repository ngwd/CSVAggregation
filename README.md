# Exyn's C++ Programming Interview
Writing code on a white board is stressful and it doesn't accurately represent the work you'd be doing at Exyn. Instead of white board questions we'll ask you to solve the problem described below. 

The exercise has two parts. First we'll ask you to solve it at home in a Linux environment. We expect that solving this problem will take around an hour or two. When you are finished, generate a patch file and send it to us. Here are some of the things we'll be looking for in your solution:

- Can you setup and use this workspace?
- Can you create a reasonable solution to the problem? Does it work?
- Did you add any unnecessary complexity?
- Do you understand the data?
- Is it well tested?
- Do you use modern c++ features?
- Did you maintain project hygiene, treating this like "real world" software that you and your team could maintain in perpetuity? 

After you submit your solution someone from Exyn will review it and your contact at Exyn will contact you to discuss the next steps.

## Problem Description
Included in this repository is a data set taken from the [City of Philadelphia's Open Data portal](https://opendataphilly.org/). It has parking violation data from the City of Philadelphia in CSV format.

You are being asked to write a command line tool in C++ that transforms the individual tickets into a daily aggregate, with the count of tickets, high, low and average of the fine issued, in addition the data should be broken down by each zip code the ticket was issued in. 

For example, the the following tickets where issued on a single day in the 19127 zip code:

```
anon_ticket_number,issue_datetime,state,anon_plate_id,division,location,violation_desc,fine,issuing_agency,lat,lon,gps,zip_code
8539521, 2017-11-11 07:08:00, PA, 4729992,, 100 SALAIGNAC ST,   PARKING PROHBITED,  41, PPA, 40.020192,	-75.213368,	true, 19127
8539522, 2017-11-11 07:15:00, NJ, 4729993,, 100 HERMIT ST,      PARKING PROHBITED,  41, PPA, 40.021593,	-75.212852,	true, 19127
8539523, 2017-11-11 08:16:00, PA, 4729994,, 4700 MANSION ST,    PARKING PROHBITED,  41, PPA, 40.032825,	-75.229638,	true, 19127
8539524, 2017-11-11 10:03:00, PA, 4729995,, 4300 MAIN ST,       METER EXPIRED,      26,	PPA, 40.025282,	-75.223162,	true, 19127
8539525, 2017-11-11 10:07:00, OH, 4729996,, 4400 MAIN ST,       METER EXPIRED,      26,	PPA, 40.0264,	-75.225562,	true, 19127
8539526, 2017-11-11 10:09:00, PA, 4729997,, 4400 MAIN ST,       METER EXPIRED,      26,	PPA, 40.026373,	-75.225662,	true, 19127
8539527, 2017-11-11 10:23:00, PA, 4729998,, 200 RIPKA ST,       OVER TIME LIMIT,    26,	PPA, 40.031058,	-75.228207,	true, 19127
8539528, 2017-11-11 10:24:00, NY, 4729999,, 200 RIPKA ST,       OVER TIME LIMIT,    26,	PPA, 40.031355,	-75.227825,	true, 19127
```

Then the expected values for count of tickets, high, low and average for this day would be:

- count of tickets: 8.0
- high: 41.0
- low: 26.0
- average: 31.6

### Output format
The program should read the data from a file specified in the first argument and output the aggregated data to STDOUT in CSV format. The resulting CSV should follow this format:

```
Zip Code,Date,Min Fine,Max Fine, Avg Fine, Count of Tickets
19127, 2017-11-02, 41.0, 41.0, 41.0, 1.0 
19127, 2017-11-11, 26.0, 41.0, 31.6, 8.0 
...
```

### Some Considerations

- For this first part of the exercise, you can assume the input data will be consistent with the example
- The output must be valid CSV, with the correct number of columns
- The output must include a header, as specified above
- The output must use the same date format as the input (i.e. MM/DD/YYYY)
- The output must round numbers to 1 number after the decimal, appending trailing zeros to whole numbers. Examples include:
 - - 30.3 -> 30.3
 - - 73 -> 73.0
 - - 12.34 -> 12.3
- The sort order of the output data rows is not important
- Ensure that the tests are passing (via make test) and the program runs (via make run) when you submit your solution
- We're looking for a result that is correct, reasonably performant, and that other software engineers would be happy to maintain.

## Problem Environment
This repository has a Makefile, prepared for a modern Linux environment. There is a requirement of GCC version 7 or higher existing on your host environment. The make file has different targets for running tests and executing the program. You will probably need to make some changes to the Makefile throughout the exercise. If you are not familiar with make you can find a getting started document [here](https://makefiletutorial.com/#getting-started).

To build the project run `make` in the root directory of this repository. 

To run the tests run `make test`.

To run the program with the sample data run `make run`.

## Submitting Your Solution
If you've applied to a job at Exyn and asked to submit your solution this is how:

### Run `make patch`
When you've finished solving the problem and are happy with your work run `make patch` from the command line in the root of this project repository. This will generate a file named `exyn_interview_solution.patch`.

### Submit Your Solution
Once you've generated a patch file please submit it via the link you were provided with by your Exyn contact. Afterwards a software engineer from Exyn will look at your solution. If you run into problems, or have questions please feel free to reach out to your contact with questions.