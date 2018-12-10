---
title: Evaluation of the NESizer2 method as a means of wrapping embedded legacy systems
documentclass: article
csl: ieee.csl
---

# Abstract

# Introduction

When integrating legacy information system components into a modern system, one of several usual approaches is to create an interface for the modern system to control or communicate with the legacy component. This approach is known as wrapping.[@Bisbal1999;@Sneed2000] Wrapping as a concept could be adapted for legacy hardware in embedded systems, to enable the original and proven functionality of the outdated system, by providing an interface to control the legacy components. There are few documented examples of migration of legacy systems that include the incorporation of the system including the hardware platform, and the few examples that exist are often designed with a specific functionality in mind. The NESizer2 project details a method wherein a modern microcontroller is used to wrap certain functionalities of the microprocessor used in a Nintendo Entertainment System, by dynamically injecting instructions to the microprocessor like an emulated ROM.[@NESizer2_GitHub] This thesis will evaluate how this method could be expanded upon to allow for a general use case of the legacy component, and to evaluate how well the method performs as a means of wrapping.

## Background

Computer based information systems are an invaluable asset for modern enterprises and corporations. The use of information systems can range from data storage, analysis and organization, to communication systems such as mail servers. With continued updating and upgrading of these systems they become increasingly complex, and as technology evolves the existing technology of the systems are quickly rendered obsolete, and "eventually, the existing information systems become too fragile to modify and too important to discard" [@ComellaDorda2000], and as such it can be considered a legacy system. [@seacord2003] At this point the legacy systems must be modernized, or migrated into a more modern system to remain viable.

When incorporating legacy information systems into modern systems, there are usually three popular approaches: redevelopment, wrapping and migration.[@Bisbal1999, p.2-4] While redeveloping an entire system is usually the best option in the long run, it is also the most expensive and risky. Therefore, migration is usually a more popular method, as it provides an interface to control the legacy components, while retaining its data and functionality. However, migrating systems to a modern platform can lead to unexpected behaviour, with a notable example being NASA’s Ariane 5 flight 501.[@Dowson1997;@Ariane-Board-Report]

When redevelopment and migration is too risky or expensive, wrapping offers a cost-effective option with less risk. It surrounds existing data, systems and interfaces with new interfaces, giving the legacy components a "new and improved" look [@Bisbal1999, p. 3], and lets organizations reuse trusted components in a new, more up-to-date manner.
While these methodologies and frameworks usually focus on legacy software systems, they are very much applicable to hardware systems and components as well.

One way of wrapping a legacy hardware component is detailed in the hobby project NESizer2 by _Johan Fjeldtvedt_.[@NESizer2_GitHub] The project is a MIDI controlled synthesizer, using the original _Audio Processing Unit_ (APU) found embedded in a RP2A03 microprocessor - the microprocessor used in the Nintendo Entertainment System. In his method he uses a modern microcontroller to handle the normal functionality of a MIDI-controller aswell as controlling the RP2A03 by dynamically injecting instructions into the microprocessor when the APU is needed.

## Problem

To address the issues of unexpected behaviour in an otherwise proven, well functioning system, the solution could be to keep only the crucial legacy components including their hardware platforms, and provide an interface for a modern system to control them - creating a wrapper for both hardware and software. While there exists implementations of similar approaches, they are often designed with a specific functionality of the legacy component in mind, and as such does not provide a method of controlling the component for a general use case. This poses the question;

Could these specific implementations be generalized into methods of controlling a legacy component, without any specific use case in mind? If so, how well do they perform as a means of modernization?

To try and answer these questions, we will investigate the method used in the NESizer2 project, hereafter referred to as _The NESizer2 method_, to see if it can be expanded upon to be used as a _wrapper_ for the RP2A03 microprocessor. The method is considered a wrapper if it can allow a general use case of the microcontroller - specifically if it can successfully utilize the entire instruction set of the processor, thereby allowing any RP2A03 program to be run through the wrapper.

## Purpose

The purpose of this report is to investigate how the NESizer2 method performs as a means of modernization, by repurposing the method to handle the entire instruction set of the RP2A03/6502 microprocessor, and measuring its performance in speed of execution aswell as investigating the complexity of implementing the method.

The purpose of the work is to provide some insight to how well a relatively simple method of wrapping an outdated microprocessor can be expanded upon to function as method of modernizing a legacy embedded system. Although our work is very basic and does not cover the entirety of how to wrap a whole legacy system, we hope our findings can be used as a future reference for others interested in modernizing embedded hardware.

## Goal

The goal with the work is to provide insight into how an existing method of controlling legacy hardware can be extended to allow for general usage, and to give a performance evaluation of the method. This can hopefully give an indication of their usefulness as a method of modernizing an information system that uses legacy components.

### Social benefits, ethics and sustainability

If it is possible to wrap entire embedded legacy systems with relatively easy means, it could provide an alternative for businesses that are dependent on legacy embedded components to upgrade their systems without having to invest in, what most likely would be, expensive migrations, and with minimized risk - as wrapping would keep the legacy components intact. We also hope that our work can contribute to other research that aims towards a more sustainable solution than discarding still functioning computer systems, which is becoming an increasing threat to our environment.[@SVTC_PoisonPCs_ToxicTVs]

We acknowledge that our research could contribute to the continued use of legacy hardware. While the process of discarding obsolete hardware etc. for new parts can have a negative impact on the environment, it is also important to note that upgrading hardware could prove to be a better solution, as much research and development is aimed towards lower power consumption and with a more up-to-date view on sustainable engineering.

## Methodology

In order to expand our knowledge and theoretical background in the field of research and define the research objectives, literature studies were conducted on several occasions during the research. A literature study is the systematic and methodical analysis of literature for a scientific purpose.[@LUI_litteraturstudie]

A literature study was also conducted to decide on a suitable scientific method under which to conduct the research. The scientific method acts as a framework or guidance for the researcher to conduct their research in a well defined and systematic way, based on the works and experiences of researchers before them, and it is crucial to a research in order to ensure quality and correctness of gathered results and analysis.

We found that experimental research was most suited to the nature of our research. The experimental approach allows for observing how a system's behavior changes as one variable is manipulated while other variables are kept stable [@IntroToResearch], and as such it is suitable for analyzing performance of a system.[@Hakansson_Portal]

## Stakeholder

## Delimitations

The scope of this report is limited to the design and performance analysis of the NESizer2 method when it has been expanded to handle the entire 6502 instruction set, on a RP2A03 microprocessor. Performance evaluation has been limited to speed of execution per cycle across different types of test programs, as well as the response time from when an instruction is issued until it is performed by the RP2A03. The details of the implementation and evaluation criteria can be found in subsequent sections.

For a better indication of how well the communication method studied in our research can be adopted for other microchips/hardware and for a better picture of the behaviour of these communication methods on other systems, it would have been beneficial to implement them for two or more devices with different architectures. We have compared the performance in execution time against a theoretical execution time of one instruction per second, however measuring the execution speed of the RP2A03 used "as intended" with a ROM could possibly have been more insightful.

## Disposition

# Background theory/Technical background

This chapter provides an introduction of what legacy hardware means, and also discusses how legacy hardware is used in modern systems and particularly how old hardware is still used to create and produce retro-sounding music. It also introduces a hobby project that sparked the idea for this research. The second part of this chapter discusses some earlier work related to the research problem, and work that was used as a basis for the communication methods designed for this research.

## Legacy
Legacy is a term used in computing to mean "of, relating to, or being a previous or outdated computer system".[@Webster_legacy] This could, for example, be computer systems or components that might have had a widespread usage or been considered a standard in the past, but are no longer manufactured or maintained by the producer.

## Trackers and the Chiptunes scene

In the mid 1980’s, a type of music sequencer commonly referred to as a "tracker" appeared on the personal computing markets. Today, a community colloquially named the "chiptune scene" consisting of musicians and retro enthusiasts fascinated with the characteristic sounds of the old video game consoles, create and perform their own music with these trackers. While many artists use software that can emulate the sounds of these machines on modern systems, it is often considered high status to create the music directly on the old hardware. An often recurring example representative of the scene is the tracker software LSDj [ref], written for the Nintendo Game Boy. Its portable nature makes it an ideal option for artists, being able to carry their "instrument" anywhere with ease.

We wanted to, as a hobby project, develop a prototype for a portable music tracker, similar to the Game Boy and LSDj, using the characteristic sound from the popular Nintendo Entertainment Systems (NES) processor Ricoh RP2A03. In our research, we realized that it would be beneficial if we could write the tracker software for a modern microcontroller that would in turn control the Ricoh chip as a slave unit. This would give us all the expansive capabilities of a modern microcontroller, while also providing us with the actual audio output of the NES.

We realized that our need to control the Ricoh chip in this fashion could also be applicable to other legacy systems that are in need of upgrades, and where emulation is not a viable option.

## MOS Technology 6502 architecture and the Ricoh RP2A03

### MOS Technology 6502
The MOS Technology 6502 microprocessor and architecture was introduced on the market in 1975. It gained almost instant popularity due to its competative performance for a cheaper price than its competitors.[@IEEE-HoF-6502]

... TODO (Explain 6502 addressing modes)
The 6502 microprocessor contains instruction families and addressing modes to control every part of the architecture. Other than instructions which target basic CPU functionality (such as controlling program counter, reading status register etc.), there are groups of instructions to perform operations with the accumulator and memory. Included in these groups are immediate, zero page and absolute addressing. Immediate addressing , and takes one operand in addition to its opcode. Zero page addressing utilizes the address range `0x0000-0x00FF`, meaning it also only needs one operand. Absolute addressing has access to the entire available memory range (`0x0000-0xFFFF`), meaning it together with its opcode will require two additional operands.
A full, detailed explanation of all of the available addressing modes can be found in the MC6500 Microcomputer Family Programming Manual.[@6502ProgManual] This research will utilize three of the addressing modes of the 6502 microprocessor, which are immediate, zero page and absolute addressing. See [Delimitations] for further details on the choice of instruction families.

### Ricoh 2A03
The microprocessor that was used in the Nintendo Entertainment System was a Ricoh RP2A03 chip. [TODO fix @NintendoPatent] The RP2A03 is a proprietary chip based on the MOS Technology 6502 microprocessor architecture, with the difference that it has an added Audio Processing Unit (APU), and it does not support _decimal mode_[^decimal-mode] that would normally be available on a 6502 architecture.[@IEEE-HoF-6502]

## ATmega328P

The ATMega328P is an 8-bit, low-power CMOS microcontroller based on the AVR RISC architecture, with a throughput of up to 1 MIPS per MHz.[@ATMega328_Datasheet] It is an easy-to-program, multi-purpose microcontroller that is included on the Arduino Uno and Nano microcontroller boards. It contains 32 KBytes ISP flash memory with true read-while-write operation, 1 KByte of EEPROM and 2 KByte of internal SRAM. It's 23 GPIO pins with programmable peripheral interfaces including SPI, I^2^C and USART makes it an excellent light-weight microcontroller for relatively small scale projects.

## Related work
TODO
* NESizer2
* Shared memory, injection-grejen
* Andra artiklar vi hittade tidigare

# Development and methodology

This chapter gives an introduction to experimental research and how it can be used in system performance comparison and analysis, followed by a theoretical background to agile development.

## Experimental research

During the research, a research method was applied to facilitate the process of analyzing and evaluating our implementation. A literature study was conducted in order to find an appropriate research method and strategy. The research methodology was chosen with the research question in mind; how to analyze and evaluate a system performance. The two main categories of research methodology are _quantitative_ and _qualitative_ research, which are separated by their founding philosophical assumptions. The qualitative research methodology assumes that observations, and importantly the conclusions drawn from them, are by their nature connected and dependent on prior knowledge and skill of the researcher and that the same observations might lead to different conclusions depending on the researcher. Qualitative research is mainly inductive in its nature, and the researcher will use their observations to infer _possible_ hypotheses based on observations. Quantitative research, on the other hand, stems from positivism; the philosophical stand point that all things are governed by principles or laws (e.g. natural) and as such it is possible for researchers to observe these laws to draw conclusions in their research.[@IntroToResearch, p. 23] Contrary to qualitative research, a quantitative approach is generally deductive, and is often aimed to confirm or deny a hypotheses that has been stated beforehand.[@Hakansson_Portal;@IntroToResearch]

One example of quantitative research is _experimental research_. Experimental research is a strategy where the researchers try to control all variables that can affect the outcome of an observation. By methodically manipulate the state of one variable at a time, while keeping other variables stable, it is possible to understand how different variables affect the phenomenon that is to be researched.[@Hakansson_Portal;@IntroToResearch, p. 26] As its main method of data collection, experimental research relies on experiments that are performed in this fashion. The gathered data can then be analyzed and used as a basis for conclusion to confirm or deny the stated hypotheses. In computer systems this method can be used to isolate the behaviour of the system for a certain input or event, and can be a useful method to analyze system performance.[@Hakansson_Portal]

We chose to work according to the experimental research strategy, seeing as it is a suitable approach to analyze computer systems. To analyze the performance of our implementation we have chosen to observe how _response time_ and _time of completion of a program_ varies with respect to different sets of instructions and the program length. Because of limitations in our implementation in its current state, further described in subsequent chapters, we hypothesize worse performance than if the chip could read instructions directly from a ROM, as intended. However, if the implementation is capable of executing the entire instruction set as expected, we believe that there are many areas of the implementation that can be optimized for better performance with relative ease.

### Experimental techniques in computer system performance research

The development of computer systems has long been an area heavily driven by the marketplace. In order to be competitive on the market, a computer system has to either provide the highest performance, or the most cost effective computing engines. This means that as developers of computer systems, we need to succesfully "understand and then eliminate the system bottlenecks that prevent us from exploiting the available technologies". To gain a good understanding of how modern computer systems behave, and to localize the source of bottlenecks in a precise manner, experimental techniques are required.[@P1990] In our research we have chosen one of these techniques when designing experiments to gather data and analyze the performance of our implementation - _hardware monitoring_.[@P1990] The reason for chosing only one of these techniques is that the scope of interest for this research is mainly to see how well the embedded hardware functions as a means of wrapping older hardware, and to deduce this we can gain sufficient data by monitoring the timing of hardware signals. Techniques that could be used to further develop the systems are discussed in section [Future work].

## Design and software development

This section provides a brief introduction to agile development and Scrum. These development frameworks was used during the research work to facilitate the design and development process of the research.

### Agile development

The term agile, meaning "to have a quick resourceful and adaptable character"[@Webster_agile], was made popular in the software development circuit by the Manifesto for Agile Software Development.[@AgileManifesto] The manifesto describes a model that, in contrast to traditional models for software development, embraces the fact that product description and requirements will most likely change during a development process, and adapts accordingly. It encourages building projects around motivated individuals, and promotes self-organization, continuous team meetings to reflect on the work that has been done, and regularly delivering work-in-process products to the product owner.

The agile software development model has spawned a number of frameworks to uphold the manifesto, including Extreme Programming (XP) and Scrum. These frameworks have helped set the standard of agile development, and has as such gained an immense foothold in the software development field. More recently, many universities are offering courses in the agile software development model, with research continously being done on how to effectively do so.[@AgileTeaching]

### Scrum

As previously mentioned, Scrum is one of many frameworks that applies/upholds/maintains(?) the Manifesto for Agile Software Development. The creators Jeff Sutherland and Ken Schwaber define Scrum as the following[@ScrumGuide, p. 3]:

> _"Scrum (n): A framework within which people can address
> complex adaptive problems, while productively and creatively
> delivering products of the highest possible value."_

Scrum utilizes an iterative, incremental approach to manage risks, and to dynamically develop a solution to a problem. The project is broken down into a set of time boxes known as "Sprints". The creators of Scrum recommend a sprint length of no longer than a month [@ScrumGuide, p. 8], however the author of the popular Scrum introduction book "Scrum and XP from the Trenches" Henrik Kniberg recommends new Scrum teams to experiment with sprint lengths until they find a time frame that the team feels comfortable with.[@ScrumKniberg, p. 22]

Each member of the Scrum team is assigned a role. These roles include product owner, developers and Scrum master. Each role have a specific set of tasks to fulfill.

* Scrum Master
  - Responsible for ensuring that Scrum is understood and enacted [@ScrumGuide, p. 6] by making sure that the each member of the team follows the Scrum theory, practice, and rules.
* Product Owner
  - Responsible for maximizing the value of the product and the work of the development team[@ScrumGuide, p. 5], and of managing the so called "Product Backlog", which contains items/tasks/requirements(?) that are to be completed in order for the product to meet the definition of done.
* Developer
  - Professionals who do the development work by delivering potentially releasable software at the end of each sprint.[@ScrumGuide, p. 6]

The framework employs four formal events that help make sure that the team can deliver at the end of each sprint. These events are known as Sprint Planning, Daily Scrum, Sprint Review and Sprint Retrospective.[@ScrumGuide, p. 7]

* Sprint Planning
  - The Scrum team collaboratively decides on what can be delivered at the end of the sprint by moving tasks from the Product Backlog to a Sprint.[@ScrumKniberg, p. 24] A Sprint Goal is then created, which is a goal set by the team that can be reached by implementing the items in the Sprint backlog.
* Daily Scrum
  - A short meeting, usually around 15 minutes, where the team discusses the work that they will do on that day. This is done in order to synchronize team members, improve communication and improve the teams knowledge.
* Sprint Review
  - Helt at the end of each sprint, where the Scrum team and possible stakeholders collaborate and discuss what was done in the sprint. The attendees inspect the Product Backlog and any changes that was made, decide on what could be done in the next sprint in order to optimize value. The meeting is held with the intention of generating feedback.
* Sprint Retrospective
  - Held after the Sprint Review and before the next Sprint Planning meeting. It is held in order to inspect how the last Sprint went with regards to the team members, their relationships, the process and tools. The team tries to identify what went well, and what can be improved, with the aim to create a plan that improves performance in the next sprint.

## Evaluation criteria

* Performance in speed
	+ How fast is the wrapper method compared to non-wrapped?
	+ Measure time of completion of programs categorized in types of operations
* Functionality as a wrapper method
	+ Same behaviour as non-wrapped?
	+ Response time


# Project work/Development

This section details the project work, including literature studies and the hardware and software implementation/design process as governed by the Scrum framework, and the design and implementation of experiments according to the experimental research approach. It has been structured to closely follow the steps of the project work in chronological order.

## Literature study

The project work started with a literature study, to gain knowledge on related work and theoretical background knowledge in the field of modernizing legacy hardware and legacy hardware used in modern applications. The search was performed using mainly the following databases of scientific publications: (i) IEEE Xplore, (ii) ACM Digital Library and (iii) Google Scholar. Additional searching tools used was simple internet searching tools such as _Google_, which could often provide ideas for additional keywords used when further searching in the databases. The results from this literature study was searched for using keywords: modernization/modernizing, legacy, hardware, microprocessor, computer. Based on the results of this search we further defined our keywords to target specific methods that seemed relevant, in order to find references on related previous work. The keywords used in this search was: legacy, microcontroller, microprocessor, master, slave, injection, wrapping, shared memory. The results from both searches was selected with title, abstract and publication year taken into consideration. Most of the related work was found to be older than 10 years, but considering that the articles mentioned methods of controlling legacy hardware, and that the problem of upgrading/modernizing hardware is generally a problem for machines older than 10-20 years, we found them relevant to the research.

Another literature study was conducted in order to gain further knowledge on experimental research, how it is used in performance comparison and evaluation, and general information on scientific methodology and how it is used in research. The keywords used in this search was: experimental, research, methodology, computer, system, performance. The search was conducted over the same databases mentioned above, and evaluated and selected using the same process. Anne Håkansson's article _Portal of Research Methods and Methodologies for Research Projects and Degree Projects_[@Hakansson_Portal] mentions the book _Introduction to Research in Education_[@IntroToResearch] as a source, and it has proved to be of great help when trying to understand what experimental research means, and how it can be used as a research strategy/methodology.

## Designing experiments

The experiments had to be designed not only to reflect our evaluation criteria, but in accordance with the experimental research approach they had to be designed around understanding how _the single independent variable_ and _the single significant variable_ affect the system performance.

When designing the experiments to evaluate performance in time, we used the categories of programs discussed in section [Evaluation criteria] and identified a set of all controllable variables that could have a significant impact on the results from each category. The variable chosen were:

i. Length of program, as measured in number of instructions
ii. Selection of timing for reset function
iii. Using emulated ROM functions or "pure" assembly

For each category we then designed an experiment that would test the performance when changing each of these variables while keeping the others stable.

To address the evaluation criteria _behaviour_, we included a data validation test to each experiment. The data validation is simply to verify that all test programs produced the expected output at each RP2A03 write cycle.

The outline of the experiments are further detailed in section [Experiments phase].

## Hardware and Software implementation process

When planning the project work we decided to use agile development and the framework _Scrum_ to govern the design and implementation process. We chose divide design and implementation work into categories _hardware_ and _software_, with one person responsible for each category. Before our first sprint we set up milestones and goals, and created a backlog of stories, or tasks, that would work towards reaching set goals. This backlog is what we used to define tasks to include in each sprints.

The sprint goals were then set to reflect stages of iteratively increasing implemented functionality in the method, and stories and tasks for that specific sprint were then chosen to reflect the sprint goal. For each sprint we also defined a set of one or more deliverables that should represent the result of that sprint. A detailed description of the work concluded in each sprint during the implementation process will be given below, and a summary of the sprint goals and deliverables can be seen in Table 1.

Sprint #	Goal								Deliverables
----------	------------						--------------------
1			Research and Design phase done		Research and design documents
												for hardware and software
2-3			Basic communication working			Components, circuitry, and basic
												software needed for communication
4			Sending instructions				Hardware for debugging, Software
												for sending instructions
5			Entire instruction set working		Software supporting entire instruction set

Table: Overview of sprints and deliverables

### Sprint 1, Research and design phase

The research and design phase included research the NESizer2 software and hardware implementation. This research was made in order to pinpoint the parts from NESizer2 that we would need, and what modifications had to be done to it. We found that we could use the assembly routines (which was at the heart of the communication with the RP2A03) together with a simple 8-bit databus through an 8-bit latch to control flow of instructions, as a foundation for our implementation. This is detailed in the circuit diagram found in Appendix A.

We also made research on the RP2A03 microprocessor, but since the RP2A03 is a proprietary chip, owned by Nintendo, there was not much official information available. However, a community of NES enthusiasts has, through reverse engineering, gathered much information on the processor on their forums and wiki-site _Nesdev Wiki_.[@NesdevWiki] Through these channels we were able to learn that the chip differs little from the MOS Technology 6502 architecture, and as such we could learn much about the chip through official 6502 hardware and software development guides.

Hardware design work then included research and decision of what hardware components were needed for our implementation, and finally to create a circuit diagram that we could use when building. Software design work resulted in a program flowchart and diagram of software components.

### Sprint 2 & 3, Basic communication

The first part of the sprint was aimed at basic testing of components. Testing the RP2A03 was hard without any specialized tools, but we found a online document detailing a simple method of testing (i) power on and (ii) data bus of 6502.[@6502Test] The method mentions how to test this by using an array of LEDs on the address pins, but we decided to expand the test to observe the chip with an oscilloscope; we monitored the Read/Write and Clock output pin of the chip to confirm that different inputs produced expected sequence of RW signals. This gave us the confidence that the data bus and instruction parsing functioned, at least on some level.

After component testing was concluded, the next step was building a simple circuit of components that, together with basic software and a simple test program, could confirm that basic communication between the microcontroller and RP2A03 was working as intended. The results were inconsistent and erroneous, and the build had to be debugged, which resulted in the sprint "overflowing" to the next sprint.

We finally realized that the problem was due to a misunderstanding in how the latch was used in NESizer2, and a simple edit to the circuitry resulted in consistent expected results. This confirmed that communication between the chips worked at a basic level. Figure \ref{misplaced_latch} and \ref{misplaced_latch} illustrates the misbehaving circuit and the corrected circuit, respectively.

![The circuit with the misplaced LE pin on the 74LS373 latch .\label{misplaced_latch}]("../img/Emulated ROM/v10/emulated_rom.png")
![The circuit with the corrected LE pin on the 74LS373 latch.\label{corrected_latch}]("../img/Emulated ROM/v20/emulated_rom.png")

### Sprint 4, Sending instructions

Hardware work included implementing simple debugging and diagnostics that could be used to determine if something was not working as intended in a quick manner, and proved to be very helpful during the sprint. After concluding the hardware work of the sprint, the software work was distributed to both parties and implemented together.

The NESizer2 uses high(er) level functions for instructing the RP2A03 to play a note, or to modify the sound, etc., with the help of hardcoded assembly instructions that performed set memory operations. We wanted to extend these assembly routines to allow for any instruction to be sent, and to build our own higher level C functions that could be used in a C program for the microcontroller used in the implementation. This was achieved by categorizing the 6502 instruction set (which is the instruction set used by the RP2A03) into instruction families that use the same number of operands. At this stage we chose to focus on three main families; (i) Immediate operations, (ii) Absolute addressing memory operations, and (iii) Zero Page addressing memory operations. A summary of the characteristics of these families can be found in Table 2.

Instruction family		Size of instruction [B]			# Cycles / instruction		R/W Cycle sequence
----------------------  --------------------------- 	--------------------------	----------------------
Immediate				2								2							R, R
Absolute				3								4							R, R, R, R/W
Zero page				2								3							R, R, R/W

Table: Instruction families and their characteristics

These characteristics was then used to redesign the assembly routines and to build the functions mentioned above. When implemented it was possible to send any opcode[^opcode] together with any operand, handled properly by the assembly routines.

### Sprint 5, Entire instruction set

The first part of this sprint focused on further extending the functionality of the previously written instruction handling, allowing a 6502 program to be stored and parsed as an array of byte instructions, functioning as a simple emulated ROM. This simplified the setup routine that had to be performed on boot of the RP2A03, as it could simply be written into the ROM, followed by the rest of the RP2A03 instructions we wanted to run.

When analyzing the 6502 instruction set further, we concluded that we were not interested in implementing support for branching instructions that conditionally jumps the program counter. This is because the RP2A03 reads instructions directly from the microcontroller, and as such branching has no real effect.

The second part of the sprint was aimed at thoroughly testing the entire instruction set, making sure that all instructions produced expected results. While all operations that used the accumulator register (immediate loads, ALU [^ALU] operations, etc.) worked consistently and as expected, we noticed that seemingly _none of the memory operations worked_. This is further detailed and discussed in sections [Results] and [Conclusions & discussion].

The sprint, and some continuous work during the experiments phase, was concluded without managing to resolve this issue.

## Experiments phase

In order to measure time according to our criterias, we attempted to set up the SPI peripheral on the Atmega328P. Unfortunately the programmer we used did not support two-say SPI communication, which forced us to further extend the implementation with a second microcontroller unit.

The implementation was extended in hardware and software to include simple communication with an Arduino M0 Pro board, which was used to measure time. The choice of the Arduino M0 Pro as a hardware timer was the increased resolution of time, and the fact that it ran on a clock with more than double the frequency of the wrapper system, further increasing accuracy of the measurements.

To measure communication timing and cycles between instructions, we used a digital logic analyzer to monitor digital output. The data from the analyzer could then be collected both numerically (in the form of CSV) and as diagrams. On the RP2A03 we chose to monitor all bits of the data bus, as well as the R/W and output clock pins, and on the microcontroller we monitored a signal pin to measure response time.

The experiments phase was conducted according to design, with the exception that any test cases involving memory operations could not be confirmed to produce expected results. The experiments were performed according to the following steps:

1. Validate data output and record response time when all variables are at default values
2. Measure time of completion as program increases in length
3. Measure time of completion as time interval of reset increases
4. Measure time of completion when program is called with emulated ROM functions
5. Measure time of completion when program is called without emulated ROM functions
6. Switch to next category and repeat process until all categories have been tested

For step 1 we used a digital logic analyzer to measure the bit value on the output of the RP2A03, in order to ensure that an expected value was output. (TODO bilder på detta). We also recorded the response time for each instruction type (TODO tabell på detta?) at the same time.
For steps 2 through 5 we utilized the Arduino M0 Pro to act as a master controller unit, which we programmed to tell the Atmega328P to start executing programs on the RP2A03 on our command, and at the same time measure the time it took for the Atmega328P to execute. Three different test programs were written for each controller device (Arduino M0 Pro and Atmega328P), each following the same pattern:
The Arduino waits for the Atmega328P to signal that it has completed its setup routine. The Atmega328P then waits for a start signal from the Arduino, which is sent upon the press of a hardware button. When the button is pressed, the Arduino sends a *go* signal to the Atmega328P and starts a timer. It then waits for the Atmega328P to signal that it has finished it execution, where upon the Arduino will stop it's timer and save the results. When the test program has finished, the Arduino outputs all the measured times. (TODO For the full test program implementations, see appendix kanske typ eller nåt?)

TODO Eventuellt fylla ut?

# Results

This chapter details the results of all tests that were performed on the system. The tests were performed as outlined in the subchapter [Experiments phase], and the test results have been labeled

1. Data validation and Response time
2. Increasing number of instructions
3. Increasing frequency of resets
4. Using ROM-Emulation functions
5. Not using ROM-Emulation functions

, respectively.

Firstly all tests are presented and compared across categories, followed by a comparison of tests for each separate category. In the category _Mixed_, only three test cases were applicable and relevant, and as such the _Mixed_ category will not show in all comparisons. (TODO speca vilka test som detta gäller, här eller i subkapitlena?)

## Comparison: Test cases

Test cases 1 through 5 are shown in figures \ref{data-validation}, \ref{increasing-instructions}, \ref{increasing-interrupt-timeout} and \ref{emurom-func} respectively. Categories are compared to each other in each figure where applicable.

![.\label{data-validation}](../tests/results/data-validation.pdf)
![.\label{increasing-instructions}](../tests/results/increasing_instructions.pdf)
![.\label{increasing-interrupt-timeout}](../tests/results/increasing_interrupt_timeout.pdf)
![.\label{emurom-func}](../tests/results/emuromfunc.pdf)

As shown in figure \ref{data-validation}, data could only be partially validated for categories _Zero Page_ and _Absolute_. They are categorized as partially validated because the instructions sent from the master unit and the execution time on the slave unit behaves as expected, but the data output after execution is not correct. As shown in table \ref{mem-error}, the `LDA (0xA5)` instruction loads the value `0x04` from Zero Page address `0x24`, however trying to store the accumulator at another memory address strangely enough outputs `0x24` from the accumulator instead.

Table: Illustrating error in memory operations \label{mem-error}

------------------------------------------------------------------------------
Data bus	Comment
--------	-------
`0xA5`		Zero Page `LDA` instruction Opcode
`0x24`		Zero Page `LDA` Operand, Zero Page Address `0x24`
`0x04`		Returned value `0x04` from memory
`0x85`		"Idle" Zero Page `STA` instruction
`0x85`		"Idle" Zero Page `STA` instruction
`0x85`		"Idle" Zero Page `STA` instruction
`0x85`		"Idle" Zero Page `STA` instruction
`0x85`		"Idle" Zero Page `STA` instruction
`0x85`		"Idle" Zero Page `STA` instruction
`0x85`		Zero Page `STA` instruction Opcode
`0x06`		Zero Page `STA` Operand, Zero Page Address `0x06`
`0x24`		Value on accumulator to be written to memory (Expected `0x04`)
------------------------------------------------------------------------------

When not performing memory operations, i.e. accumulator writes and ALU operations, all data was validated as expected, even when performing sequences of connected operations. Table \ref{xor-accumulator} shows a short sequence of instructions storing a value in the accumulator and performing an _Exclusive OR_ (`EOR`) operation on it.

Table: Illustrating a sequence of two accumulator operations \label{xor-accumulator}

---------------------------------------------------------------------
Data bus	Comment
--------	-------
`0xA9`		Immediate `LDA` instruction Opcode
`0x01`		Immediate `LDA` Operand, Immediate value `0x01`
`0x85`		"Idle" Zero Page `STA` instruction
`0x85`		"Idle" Zero Page `STA` instruction
`0x85`		"Idle" Zero Page `STA` instruction
`0x85`		"Idle" Zero Page `STA` instruction
`0x85`		"Idle" Zero Page `STA` instruction
`0x85`		"Idle" Zero Page `STA` instruction
`0x49`		Immediate `EOR` instruction Opcode
`0xFF`		Immediate `EOR` Operand, Immediate value `0xFF`
`0x85`		"Idle" Zero Page `STA` instruction
`0x85`		"Idle" Zero Page `STA` instruction
`0x85`		"Idle" Zero Page `STA` instruction
`0x85`		"Idle" Zero Page `STA` instruction
`0x85`		"Idle" Zero Page `STA` instruction
`0x85`		"Idle" Zero Page `STA` instruction
`0x85`		Zero Page `STA` instruction Opcode
`0x85`		Zero Page `STA` Operand, Zero Page Address `0x85`
`0xFE`		Result of `EOR` in accumulator to be stored in memory
---------------------------------------------------------------------

# Conclusions & discussion

# Future work

First and foremost, we acknowledge that the proposed method of hardware wrapping presented in this work shows inconclusive results. In order to tell whether this method is at all viable, it is required to (i) investigate whether the "broken" memory operations encountered during the tests was caused by a faulty RP2A03 unit, or if it was because of an error in the proposed implementation, and (ii) compare it against other methods of hardware wrapping on roughly equal level of complexity in implementation. As for case (ii), we had originally intended to compare this implementation against a different method of wrapping, which used a _shared memory_ to communicate between the master and slave unit, however we were unable to complete it because of the aforementioned problematic memory operations. The schematics for the shared memory approach can be found in [TODO rätt appendix här, nämn att man bör använda ett annat minne].

## More

software monitoring glöm ej från ref @P1990

# References
