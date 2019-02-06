# TODO
* Modify according to needs
	+ David
		- ~~Abstract "perhaps a bit short"~~
			* ~~"Perhaps a bit short"~~
				+ *Elias*: I disagree, I think it's allright.
		- Introduction & Background
			* ~~Terms like legacy, migration, wrapping could use a more detailed definition in this context, placed in background where legacy is expanded upon.~~
				+ *Elias*: Wrapping feels like it's detailed enough in the introductory background. I made some tweaks to the parts though, to give it maybe a bit more of a flow.
			* Should be a clear indication in the text where the reader can find descriptions at this stage in the report.
				+ *Elias*: Not sure where or how to do these references. Personally I don't really feel that they're that necessary.
		- Stakeholders & Delimitations
			* Second paragraph discussing speed comparison should perhaps be described in terms of what delimitations where intended and save mentioning the lack of tests for the discussion section.
				+ *Elias*: Maybe this paragraph could just be deleted? Thinking about it, it doesn't really make sense to have what's almost a discussion in the introductory chapter. If anywhere, we could bring it up in the discussion chapter!
		- Background & Related work
			* The overview of the chapter could contextualize how the subsections fit into the big picture and would benefit from being expanded along the lines of the overview for chapter 1.
				+ *Elias*: Not sure what he means by "expanded upon". Contextualize how it fits into the big picture?
			* Section 2.1 should be named something that makes it clear that it only contains a definition of the term legacy. This would also be an appropriate place to put other definitions relevant to the report as well (see above)
				+ *Elias*: Renamed to "Defining _legacy_"
			* Section 2.3 could be given additional subsubsections for immediate, zero page and absolute addressing to emphasize the importance of the terms. These concepts recur often in the text and it would be helpful for the reader to clearly see where they are initially explained.
				+ *Elias*: Made subsections of each addressing mode, and put "A full, detailed explanation..." above the new subsections.
		- Method
			* Perhaps superfluous to describe what the different research philosophies are, rather than just arguing for the chosen one (but it doesn't break the flow or hurt the report).
				+ *Elias*: I disagree, I think describing the different philosophies is a good idea.
			* ~~Scrum and agile don't have a clear connection to the research question, and as such the section describing the method isn't of obvious relevance to the work as a whole. However, it's an interesting offshoot for the curious reader. Would feel more motivated if the authors explained more how Scrum fits into the narrative, since it's central to the structure of later chapters.~~
				+ *Elias*: No clear connection to the research question, true, but very relevant to the project work. During later sections, the usage of Scrum I'd say it's pretty motivated. Added "... and more specifically the..." to the chapter overview.
			* There's room in Section 3.3 to describe how the list was produced in further detailed and which criteria (if any) were omitted.
				+ *Elias*: Could add a short paragraph after the criteria saying something along the lines of "blabla criteria chosen from personal experience" or something. I don't know.
		- Work & Development
			* Anne Håkansson is mentioned without any introduction at the end of Section 4.1 (douiukoto?)
			* "Single independent", "single significant" variable not entirely obvious, could used some explaining
				+ *Elias*: I agree, though not sure how to define the two variables in the first paragraph in the section.
			* Less technical map overviewing the layout of the hardware. (already in the text isn't it?)
				+ *Elias*: Already in the text, yes (emulated_rom_block_diagram.png).
		- Results
			* Tables 4 and 5 a little difficult to follow. Some further clarification would be useful to aid the reader in interpreting their meaning. Abbreviations like LDA, STA could be explained similar to terms like opcode, APU (footnotes, good idea)
				+ *Elias*: Added footnotes for LDA, STA and EOR. Made sure all three instructions are mentioned in text, not only in the tables. Also moved [^opcode] and [^ALU] to first time the words are mentioned.
		- Conclusions & Discussion
			* Summary at the end, gathering the broader conclusions for a simpler overview
				+ *Elias*: Would be nice. Not sure how to formulate it though.
		- Language, Grammar, Structure etc
			* When referring to other parts of text, specific sections with chapter and section number could be stated as a service to the reader. The following cases were found by David
				- 3.1 paragraph 3, reference to "Future sections"
					* *Elias*: "Subsequent chapters?" I mean, the limitations are described in more than one chapter, so it would feel weird to have it refer to one of them. I don't know.
				- ~~4.2 last sentence, refers to experiments phase section~~
				- ~~4.3.4 paragraph 3, reference to Results and Conclusion sections~~
				- ~~6.2.1 first sentence, reference to Results section~~
			* ~~In section 5.1 paragraph 1 there's a reference to [Appendix B]~~
			* ~~In chapter 7 there's a reference to [Appendix A]~~
			* ~~Section 4.2, first paragraph, sentence is long and a bit of a tounge-twister~~
	+ Daniele
	 	- Organization, structure, layout of report
			* Sometimes first-person speech is used (third-person recommended, but personal opinion)
		- Abstract
			* ~~Missing precense of "wrapping" in the keywords~~
				+ *Elias*: Already in report title
		- Literature study
			* Explanation of what a legacy component is should be expanded. Described in the abstract, then mentioned in introduction without repeating it's meaning (good point)
				+ *Elias*: Think I fixed it when fixing one of Davids points.
		- Results
			* Figures 7 and 8, legends are too close to the xlabel. Grid on preferred.
				+ *Elias*: Do we care?
		- False, incorrect, misleading or unclear aspects
			* Figure 3, pins text not very readable. Quality probably lost in some intermediate step. If possible, use a better quality one.
			* Hyperlinks and hyperrefs of sections and figures don't seem to work (lost because of pdfToolkit).
				+ *Elias*: Not sure if we can salvage this.
		- Spelling, punctuation, grammar
			* ~~6.1.5 Programming Toolchain: Change 'micro controllers' to 'microcontrollers' in rest of text~~
			* ~~6.1.5 Programming Toolchain: "During the research phase, we discovered _that_ that a popular..." (remove one of the "that"s)~~
			* ~~3.2.2 Scrum: Remove the question mark~~
			* ~~3.1 Experimental Research: "...with the _following_ question in mind: ." (e.g.) add _following_ and change semicolon to colon~~
				+ *Elias*: No.
			* Abstract: "This bachelor thesis project work aims _is_ to analyze one approach known as 'wrapping"..." Verb "to be" is missing (no idea what he's talking about)
				+ *Elias*: No.
	+ Noticed after delivery of draft
		- ~~Abstract: "...vår circuit..."~~
		- Wrong circuit schematics
* Deliver final
* Bränn ardiuno
