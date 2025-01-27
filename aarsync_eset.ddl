
	version("WebFG Development, Builder 6.1.0 SP1");
	
	cam "AARSYNC_AARELE_VISIBLE"
	(
		database("AARSDB") 
		record("AARELE") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY230%") eq 0
			gab 
			(
				visible 
			)
		else if proxy("%KEY230%") eq 1
			and field("LN_AARELE") true 
			gab 
			(
				visible 
			)
		else if proxy("%KEY230%") eq 2
			and field("ZB_AARELE") true 
			gab 
			(
				visible 
			)
		else if proxy("%KEY230%") eq 3
			and field("XF_AARELE") true 
			gab 
			(
				visible 
			)
		else if proxy("%KEY230%") eq 4
			and field("CB_AARELE") true 
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_ALL_AARELE_SORT_TRUE"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY230%") eq 0
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_CB_AARELE_SORT_TRUE"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY230%") eq 4
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_CB_AARELE_TRUE_VISIBLE"
	(
		database("AARSDB") 
		record("AARELE") 
		gab 
		(
			not visible 
		)
		if field("CB_AARELE") true 
			gab 
			(
				visible 
			)
	);
	
	cam "AARSYNC_DNV_AARELE_SORT_ASCENDING_TRUE"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY235%") eq 1
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_DNV_AARELE_SORT_DESCENDING_TRUE"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY235%") eq 0
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_DNV_AARELE_TRUE_VISIBLE"
	(
		database("AARSDB") 
		record("AARELE") 
		gab 
		(
			not visible 
		)
		if field("DNV_AARELE") true 
			gab 
			(
				visible 
			)
	);
	
	cam "AARSYNC_EMER_AARELE_VISIBLE"
	(
		database("AARSDB") 
		record("AARELE") 
		gab 
		(
			not visible 
		)
		if field("LN_AARELE") true 
			or field("XF_AARELE") true 
			gab 
			(
				visible 
			)
	);
	
	cam "AARSYNC_FILEOLD_TRUE_VISIBLE"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if field("FILEOLD") true 
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_ID_AARELE_SORT_ASCENDING_TRUE"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY231%") eq 1
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_ID_AARELE_SORT_DESCENDING_TRUE"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY231%") eq 0
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_LINE_AARELE_TRUE_VISIBLE"
	(
		database("AARSDB") 
		record("AARELE") 
		gab 
		(
			not visible 
		)
		if field("LINE_AARELE") true 
			gab 
			(
				visible 
			)
	);
	
	cam "AARSYNC_LN_AARELE_SORT_TRUE"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY230%") eq 1
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_LN_AARELE_TRUE_VISIBLE"
	(
		database("AARSDB") 
		record("AARELE") 
		gab 
		(
			not visible 
		)
		if field("LN_AARELE") true 
			gab 
			(
				visible 
			)
	);
	
	cam "AARSYNC_MAN_AARELE_KEY236_SORT_ASC_T"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY236%") eq 1
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_MAN_AARELE_KEY236_SORT_DES_T"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY236%") eq 0
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_MAN_AARELE_KEY237_SORT_ASC_T"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY237%") eq 1
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_MAN_AARELE_KEY237_SORT_DES_T"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY237%") eq 0
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_MAN_AARELE_TRUE_VISIBLE"
	(
		database("AARSDB") 
		record("AARELE") 
		gab 
		(
			not visible 
		)
		if field("MAN_AARELE") true 
			gab 
			(
				visible 
			)
	);
	
	cam "AARSYNC_MAN_EMER_AARELE_VISIBLE"
	(
		database("AARSDB") 
		record("AARELE") 
		gab 
		(
			not visible 
		)
		if field("MAN_AARELE") true 
			gab 
			(
				visible 
			)
	);
	
	cam "AARSYNC_MAN_NORM_AARELE_VISIBLE"
	(
		database("AARSDB") 
		record("AARELE") 
		gab 
		(
			not visible 
		)
		if field("MAN_AARELE") true 
			gab 
			(
				visible 
			)
	);
	
	cam "AARSYNC_NETSUB_AARELE_GT0_VISIBLE"
	(
		database("AARSDB") 
		record("AARELE") 
		gab 
		(
			not visible 
		)
		if field("NETSUB_AARELE") gt 0
			gab 
			(
				visible 
			)
	);
	
	cam "AARSYNC_NETSUB_AARELE_SORT_ASCENDING_TRUE"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY233%") eq 1
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_NETSUB_AARELE_SORT_DESCENDING_TRUE"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY233%") eq 0
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_NORM_AARELE_VISIBLE"
	(
		database("AARSDB") 
		record("AARELE") 
		gab 
		(
			not visible 
		)
		if field("XF_AARELE") false 
			gab 
			(
				visible 
			)
	);
	
	cam "AARSYNC_SCAPI_ERROR"
	(
		database("AARSDB") 
		record("AARELE") 
		gab 
		(
			not visible 
		)
		if field("SPIMSG_AARELE") not eq "" 
			gab 
			(
				visible 
			)
	);
	
	cam "AARSYNC_SEV_MESSAGE_COLOR"
	(
		database("AARSLG") 
		record("MESS") 
		gab 
		(
			foreground("236,233,216") 
			background("0,0,0") 
			visible 
			not blink 
		)
		if field("SEV_MESS") eq "W" 
			gab 
			(
				foreground("255,255,0") 
				background("255,255,255") 
				visible 
				not blink 
			)
		else if field("SEV_MESS") eq "E" 
			gab 
			(
				foreground("255,0,0") 
				background("255,255,255") 
				visible 
				not blink 
			)
		else if field("SEV_MESS") eq "F" 
			gab 
			(
				foreground("255,0,0") 
				background("255,255,255") 
				visible 
				blink 
			)
	);
	
	cam "AARSYNC_SMAP_AARELE_SORT_ASCENDING_TRUE"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY234%") eq 1
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_SMAP_AARELE_SORT_DESCENDING_TRUE"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY234%") eq 0
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_SMAP_AARELE_TRUE_VISIBLE"
	(
		database("AARSDB") 
		record("AARELE") 
		gab 
		(
			not visible 
		)
		if field("SMAP_AARELE") true 
			gab 
			(
				visible 
			)
	);
	
	cam "AARSYNC_TYPE_AARELE_SORT_ASCENDING_TRUE"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY232%") eq 1
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_TYPE_AARELE_SORT_DESCENDING_TRUE"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY232%") eq 0
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_VISABLE_IF_SCAPI_TRUE"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if field("SCAPI") true 
			gab 
			(
				visible 
			)
	);
	
	cam "AARSYNC_VISABLE_IF_STDOUT_TRUE"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if field("STDOUT") true 
			gab 
			(
				visible 
			)
	);
	
	cam "AARSYNC_XF_AARELE_SORT_TRUE"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY230%") eq 3
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_XF_AARELE_TRUE_VISIBLE"
	(
		database("AARSDB") 
		record("AARELE") 
		gab 
		(
			not visible 
		)
		if field("XF_AARELE") true 
			gab 
			(
				visible 
			)
	);
	
	cam "AARSYNC_ZB_AARELE_SORT_TRUE"
	(
		database("AARSDB") 
		record("ITEMS") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY230%") eq 2
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	
	cam "AARSYNC_ZB_AARELE_TRUE_VISIBLE"
	(
		database("AARSDB") 
		record("AARELE") 
		gab 
		(
			not visible 
		)
		if field("ZB_AARELE") true 
			gab 
			(
				visible 
			)
	);
	
	cam "TEST"
	(
		database("AARSDB") 
		record("AARELE") 
		gab 
		(
			not visible 
		)
		if proxy("%KEY230%") eq 0
			gab 
			(
				visible 
			)
		else if proxy("%KEY230%") eq 1
			and field("LN_AARELE") true 
			gab 
			(
				visible 
			)
		else if proxy("%KEY230%") eq 2
			and field("ZB_AARELE") true 
			gab 
			(
				visible 
			)
		else if proxy("%KEY230%") eq 3
			and field("XF_AARELE") true 
			gab 
			(
				visible 
			)
		else if proxy("%KEY230%") eq 4
			and field("CB_AARELE") true 
			gab 
			(
				visible 
			)
		else 
			gab 
			(
				not visible 
			)
	);
	gab "AARSYNC_ALARM_TEXT"
	(
		foreground("255,0,0") 
		background("0,0,0") 
		font_family("Arial") 
		font_size(12) 
	);
	gab "AARSYNC_BLACK_BOX_TITLE_BACK_GROUND"
	(
		background("0,0,0") 
		font_family("Arial") 
		font_size(12) 
		font_style("Bold")
		line_width(1) 
		fill 
	);
	gab "AARSYNC_BTN_TOGGLE_CHECK_MARK"
	(
		foreground("236,233,216") 
		background("0,0,0") 
		font_family("Book Antiqua") 
		font_size(10) 
		fill_pattern("check") 
		fill 
	);
	gab "AARSYNC_BTN_TOGGLE_FRONT"
	(
		foreground("236,233,216") 
		font_family("Courier New") 
		fill 
	);
	gab "AARSYNC_BTN_TOGGLE_NONENTER_FRONT"
	(
		font_family("Courier New") 
		fill 
	);
	gab "AARSYNC_BTN_TOGGLE_UNCHECK_MARK"
	(
		foreground("173,216,230") 
		background("0,0,0") 
		font_family("Book Antiqua") 
		font_size(10) 
		fill_pattern("check") 
		halo_style(as_line) 
		fill 
	);
	gab "AARSYNC_DATA_COLUMN_BOX"
	(
		foreground("42,42,42") 
		background("211,211,211") 
		font_family("Courier New") 
		line_width(1) 
		fill 
	);
	gab "AARSYNC_DATA_COLUMN_BOX_OTHER"
	(
		background("211,211,211") 
		font_family("Courier New") 
		line_width(1) 
		fill 
	);
	gab "AARSYNC_ENTRY_BOX_OUTLINE"
	(
		foreground("249,248,244") 
		font_family("Courier New") 
		line_width(1) 
		fill 
	);
	gab "AARSYNC_FV_ENTRY"
	(
		foreground("255,255,255") 
		font_family("Arial") 
		font_size(12) 
		font_style("Bold")
		line_width(1) 
	);
	gab "AARSYNC_FV_PM_BIG"
	(
		foreground("255,255,255") 
		background("0,0,0") 
		font_family("Arial") 
		font_size(20) 
		font_style("Bold")
	);
	gab "AARSYNC_HYPER_LINK"
	(
		foreground("173,216,230") 
		font_family("Arial") 
		font_size(12) 
		font_style("Italic")
		font_underline
		line_width(1) 
	);
	gab "AARSYNC_HYPER_LINK_SIDE_FRAME"
	(
		foreground("173,216,230") 
		font_family("Arial") 
		font_size(13) 
		font_style("Bold Italic")
		font_underline
		line_width(1) 
	);
	gab "AARSYNC_NONENTRY_BOX_FRAME"
	(
		foreground("113,111,100") 
		background("236,233,216") 
		font_family("Courier New") 
		fill 
	);
	gab "AARSYNC_PARAMS_TITLE_TEXT"
	(
		background("236,233,216") 
		font_family("Arial") 
		font_size(16) 
		font_style("Bold")
		line_width(1) 
	);
	gab "AARSYNC_POPUP_BACKGROUND"
	(
		foreground("50,50,50") 
		background("192,192,192") 
		font_family("Courier New") 
		fill_pattern("cross") 
		line_width(1) 
		line_style(dashed) 
		fill 
	);
	gab "AARSYNC_STATUS_MESSAGE"
	(
		foreground("236,233,216") 
		background("0,0,0") 
		font_family("Arial Narrow") 
		font_style("Bold Italic")
		line_width(1) 
	);
	gab "AARSYNC_STD_BTN_TOGGLE_HIGHLIGHT"
	(
		foreground("176,196,222") 
		font_family("Courier New") 
		fill 
	);
	gab "AARSYNC_STD_BTN_TOGGLE_SHADOW"
	(
		foreground("70,130,180") 
		font_family("Courier New") 
		fill 
	);
	gab "AARSYNC_STD_ENTRY_BOX_FRAME"
	(
		foreground("122,119,107") 
		background("236,233,216") 
		font_family("Courier New") 
		fill 
	);
	gab "AARSYNC_STD_TEXT_FV_BACK_WHITE"
	(
		font_family("Arial") 
		font_size(12) 
		font_style("Bold")
		line_width(1) 
	);
	gab "AARSYNC_TEXT_BACKGROUND_UNDERSCORE"
	(
		foreground("249,248,244") 
		background("0,0,0") 
		font_family("Arial") 
		font_size(12) 
		font_underline
	);
	gab "AARSYNC_TEXT_BOX_HEADER"
	(
		foreground("236,233,216") 
		background("0,0,0") 
		font_family("Arial") 
		font_size(12) 
		font_style("Bold Italic")
		font_underline
	);
	gab "AARSYNC_TEXT_COLUMN_HEADERS_SORT"
	(
		background("236,233,216") 
		font_family("Arial") 
		font_size(12) 
		font_style("Bold")
		font_underline
		line_width(1) 
	);
	gab "AARSYNC_TEXT_TITLE"
	(
		foreground("255,255,255") 
		background("0,0,0") 
		font_family("Arial") 
		font_size(18) 
	);
	gab "AARSYNC_TITLE_HDR_BG_BEIGE_DASHED"
	(
		background("236,233,216") 
		font_family("Courier New") 
		fill_pattern("cross") 
		line_width(1) 
		line_style(dashed) 
	);
	gab "AARSYNC_TOGGLE_CHECK_MARK_NONENTRY"
	(
		font_family("Book Antiqua") 
		font_size(10) 
		fill_pattern("check") 
		line_width(2) 
		fill 
	);
	gab "AARSYNC_TOGGLE_NONENTER_FRONT"
	(
		foreground("204,200,184") 
		font_family("Courier New") 
		fill 
	);
	gab "AARSYNC_TOGGLE_NONENTER_HIGHLIGHT"
	(
		foreground("229,229,229") 
		font_family("Courier New") 
		fill 
	);
	gab "AARSYNC_TOGGLE_NONENTER_SHADOW"
	(
		foreground("105,105,105") 
		font_family("Courier New") 
		fill 
	);
	gab "STD_BTN_CMD_FRONT"
	(
		foreground("236,233,216") 
		font_family("Courier New") 
		fill 
	);
	gab "STD_BTN_CMD_HIGHLIGHT"
	(
		font_family("Courier New") 
		fill 
	);
	gab "STD_BTN_CMD_HIGHLIGHT_FRAME"
	(
		foreground("255,255,255") 
		background("0,0,0") 
		font_family("Courier New") 
		line_width(2) 
	);
	gab "STD_BTN_POKE_FRONT"
	(
		foreground("236,233,216") 
		font_family("Courier New") 
		fill 
	);
	gab "STD_BTN_POKE_HIGHLIGHT"
	(
		foreground("249,248,244") 
		font_family("Courier New") 
		fill 
	);
	gab "STD_BTN_POKE_SHADOW"
	(
		foreground("204,200,184") 
		font_family("Courier New") 
		fill 
	);
	gab "STD_BTN_TOGGLE_CHECK_MARK"
	(
		foreground("255,127,80") 
		background("249,248,244") 
		font_family("Book Antiqua") 
		font_size(10) 
		fill 
		blink 
	);
	gab "STD_BTN_TOGGLE_FRONT"
	(
		foreground("113,111,100") 
		font_family("Courier New") 
		fill 
	);
	gab "STD_BTN_TOGGLE_HIGHLIGHT"
	(
		foreground("211,211,211") 
		font_family("Courier New") 
		fill 
	);
	gab "STD_BTN_TOGGLE_SHADOW"
	(
		foreground("105,105,105") 
		font_family("Courier New") 
		fill 
	);
	gab "STD_ENTRY_BOX_FRAME"
	(
		foreground("236,233,216") 
		background("236,233,216") 
		font_family("Courier New") 
		fill 
	);
	gab "STD_ENTRY_BOX_FRONT"
	(
		foreground("255,255,255") 
		font_family("Courier New") 
		fill 
	);
	gab "STD_FILL_PULLDOWN_MENU"
	(
		foreground("172,168,153") 
		font_family("Courier New") 
		fill 
	);
	gab "STD_FILL_SORT"
	(
		font_family("Courier New") 
		line_width(2) 
		fill 
	);
	gab "STD_POPUP_BACKGROUND"
	(
		foreground("249,248,244") 
		background("0,0,0") 
		font_family("Courier New") 
		line_width(1) 
		fill 
	);
	gab "STD_POPUP_FRAME"
	(
		foreground("172,168,153") 
		background("0,0,0") 
		font_family("Courier New") 
		line_width(1) 
	);
	gab "STD_PRIM_HEADER_BANNER_BACKGROUND"
	(
		foreground("236,233,216") 
		background("0,0,0") 
		font_family("Arial") 
		line_width(1) 
		fill 
	);
	gab "STD_PRIM_HEADER_BANNER_DIVIDER_LINE"
	(
		foreground("113,111,100") 
		background("0,0,0") 
		font_family("Courier New") 
		line_width(1) 
	);
	gab "STD_PRIM_HEADER_BANNER_OUTLINE"
	(
		foreground("236,233,216") 
		background("0,0,0") 
		font_family("Courier New") 
		line_width(1) 
	);
	gab "STD_PRIM_HEADER_DATA_DIVIDER_LINE"
	(
		foreground("113,111,100") 
		background("0,0,0") 
		font_family("Courier New") 
		line_width(1) 
	);
	gab "STD_PRIM_TITLE_REALTIME_BACKGROUND"
	(
		foreground("102,205,170") 
		background("0,0,0") 
		font_family("Courier New") 
		line_width(1) 
		fill 
	);
	gab "STD_PRIM_TITLE_STUDY_BACKGROUND"
	(
		foreground("176,196,222") 
		background("0,0,0") 
		font_family("Courier New") 
		line_width(1) 
		fill 
	);
	gab "STD_TEXT_BACKGROUND"
	(
		foreground("249,248,244") 
		background("0,0,0") 
		font_family("Arial") 
		font_size(12) 
	);
	gab "STD_TEXT_BACKGROUN_UNDERSCORE"
	(
		foreground("236,233,216") 
		background("0,0,0") 
		font_family("Arial") 
		font_size(12) 
		font_underline
	);
	gab "STD_TEXT_FV"
	(
		foreground("255,255,255") 
		background("0,0,0") 
		font_family("Arial") 
		font_size(12) 
	);
	gab "STD_TEXT_TITLE_COLUMN_HEADERS"
	(
		background("236,233,216") 
		font_family("Arial") 
		font_size(12) 
		font_style("Bold")
		line_width(1) 
	);
	
	keyset "AARSYNC_DNV_AARELE_SORT_ASCENDING"
	(
		
		key("SELECT") 
		command("SET KEY235=0;SORT/LAYER=DEFAULT DNV_AARELE") 
	);
	
	keyset "AARSYNC_DNV_AARELE_SORT_DESCENDING"
	(
		
		key("SELECT") 
		command("SET KEY235=1;SORT/LAYER=DEFAULT DNV_AARELE/DESCENDING;") 
	);
	
	keyset "AARSYNC_EDEC_ENTER"
	(
		
		key("SELECT") 
		command("SELECT/ENTER/VIEW/USER=%DISAPP%_%DISFAM%") 
	);
	
	keyset "AARSYNC_EXPORT_DB"
	(
		
		key("SELECT") 
		command("MAIL/USER=%DISAPP%_%DISFAM% EXPORT") 
	);
	
	keyset "AARSYNC_ID_AARELE_SORT_ASCENDING"
	(
		
		key("SELECT") 
		command("SET KEY231=0;SORT/LAYER=DEFAULT ID_AARELE") 
	);
	
	keyset "AARSYNC_ID_AARELE_SORT_DESCENDING"
	(
		
		key("SELECT") 
		command("SET KEY231=1;SORT/LAYER=DEFAULT ID_AARELE/DESCENDING;") 
	);
	
	keyset "AARSYNC_IMPORT_DNV_FILE"
	(
		
		key("SELECT") 
		command("MAIL/USER=%DISAPP%_%DISFAM% IMPORT") 
	);
	
	keyset "AARSYNC_MAN_AARELE_KEY236_SORT_ASC"
	(
		
		key("SELECT") 
		command("SET KEY236=0;SORT/LAYER=DEFAULT MAN_AARELE") 
	);
	
	keyset "AARSYNC_MAN_AARELE_KEY236_SORT_DES"
	(
		
		key("SELECT") 
		command("SET KEY236=1;SORT/LAYER=DEFAULT MAN_AARELE/DESCENDING;") 
	);
	
	keyset "AARSYNC_MAN_AARELE_KEY237_SORT_ASC"
	(
		
		key("SELECT") 
		command("SET KEY237=0;SORT/LAYER=DEFAULT SPIMSG_AARELE") 
	);
	
	keyset "AARSYNC_MAN_AARELE_KEY237_SORT_DES"
	(
		
		key("SELECT") 
		command("SET KEY237=1;SORT/LAYER=DEFAULT SPIMSG_AARELE/DESCENDING;") 
	);
	
	keyset "AARSYNC_NETSUB_AARELE_SORT_ASCENDING"
	(
		
		key("SELECT") 
		command("SET KEY233=0;SORT/LAYER=DEFAULT NETSUB_AARELE") 
	);
	
	keyset "AARSYNC_NETSUB_AARELE_SORT_DESCENDING"
	(
		
		key("SELECT") 
		command("SET KEY233=1;SORT/LAYER=DEFAULT NETSUB_AARELE/DESCENDING;") 
	);
	
	keyset "AARSYNC_OPEN_LOG_POPUP"
	(
		
		key("SELECT") 
		command("DISPLAY/APP=AARSYNC/FAM=%DISFAM%/VIEWPORT=AARSYNC_LOG_POPUP AARSYNC_LOG") 
	);
	
	keyset "AARSYNC_OPEN_PARAMETERS_POPUP"
	(
		
		key("SELECT") 
		command("DISPLAY/APP=AARSYNC/FAM=%DISFAM%/VIEWPORT=AARSYNC_PARAMETER_POPUP AARSYNC_PARAMETER") 
	);
	
	keyset "AARSYNC_RESET_MANUAL"
	(
		
		key("SELECT") 
		command("MAIL/USER=%DISAPP%_%DISFAM% RESETMAN") 
	);
	
	keyset "AARSYNC_SET_KEY230_ALL_AARELE"
	(
		
		key("SELECT") 
		command("SET KEY230=0;DISPLAY/NOHIGHLIGHT/APP=AARSYNC AARSYNC_DASHBOARD") 
	);
	
	keyset "AARSYNC_SET_KEY230_CB_AARELE"
	(
		
		key("SELECT") 
		command("SET KEY230=4;DISPLAY/NOHIGHLIGHT/APP=AARSYNC AARSYNC_DASHBOARD") 
	);
	
	keyset "AARSYNC_SET_KEY230_LN_AARELE"
	(
		
		key("SELECT") 
		command("SET KEY230=1;DISPLAY/NOHIGHLIGHT/APP=AARSYNC AARSYNC_DASHBOARD") 
	);
	
	keyset "AARSYNC_SET_KEY230_XF_AARELE"
	(
		
		key("SELECT") 
		command("SET KEY230=3;DISPLAY/NOHIGHLIGHT/APP=AARSYNC AARSYNC_DASHBOARD") 
	);
	
	keyset "AARSYNC_SET_KEY230_ZB_AARELE"
	(
		
		key("SELECT") 
		command("SET KEY230=2;DISPLAY/NOHIGHLIGHT/APP=AARSYNC AARSYNC_DASHBOARD") 
	);
	
	keyset "AARSYNC_SMAP_AARELE_SORT_ASCENDING"
	(
		
		key("SELECT") 
		command("SET KEY234=0;SORT/LAYER=DEFAULT SMAP_AARELE") 
	);
	
	keyset "AARSYNC_SMAP_AARELE_SORT_DESCENDING"
	(
		
		key("SELECT") 
		command("SET KEY234=1;SORT/LAYER=DEFAULT SMAP_AARELE/DESCENDING;") 
	);
	
	keyset "AARSYNC_TOGGLE_STDOUT"
	(
		
		key("SELECT") 
		command("MASK/TOGGLE=STDOUT/NOPOPUP/USER=%DISAPP%_%DISFAM%") 
	);
	
	keyset "AARSYNC_TYPE_AARELE_SORT_ASCENDING"
	(
		
		key("SELECT") 
		command("SET KEY232=0;SORT/SERVER/LAYER=DEFAULT LN_AARELE:AARSYNC_DASHBOARD_DATA ZB_AARELE:AARSYNC_DASHBOARD_DATA XF_AARELE:AARSYNC_DASHBOARD_DATA CB_AARELE:AARSYNC_DASHBOARD_DATA") 
	);
	
	keyset "AARSYNC_TYPE_AARELE_SORT_DESCENDING"
	(
		
		key("SELECT") 
		command("SET KEY232=1;SORT/SERVER/LAYER=DEFAULT CB_AARELE:AARSYNC_DASHBOARD_DATA XF_AARELE:AARSYNC_DASHBOARD_DATA ZB_AARELE:AARSYNC_DASHBOARD_DATA LN_AARELE:AARSYNC_DASHBOARD_DATA") 
	);
	
	keyset "MENU"
	(
		
		flash 
		key("SELECT") 
		command("MENU") 
		key("RIGHTCLICK") 
		command("MENU") 
	);
	
	keyset "PAGE_DOWN"
	(
		
		flash 
		key("SELECT") 
		command("SHIFT +1") 
	);
	
	keyset "PAGE_LAST"
	(
		
		flash 
		key("SELECT") 
		command("PAGE *") 
	);
	
	keyset "PAGE_UP"
	(
		
		flash 
		key("SELECT") 
		command("SHIFT -1") 
	);
	
	keyset "POPDOWN"
	(
		
		flash 
		key("SELECT") 
		command("POPDOWN") 
	);
	
	keyset "POPUP"
	(
		
		flash 
		key("SELECT") 
		command("POPUP") 
	);
	
	keyset "RIGHTCLICK"
	(
		
		flash 
		key("RIGHTCLICK") 
		command("MENU") 
	);
	
	keyset "SELECT_REMOVE_FILTER"
	(
		
		flash 
		key("SELECT") 
		command("DISPLAY/NOHIGHLIGHT/APP=%DISAPP%") 
	);
	
	menu "AARSYNC_DISPLAY_DIRECTORY"
	(
		label(localize "Dashboard") 
		command("DISPLAY/APP=AARSYNC/FAM=%DISFAM% AARSYNC_DASHBOARD") 
		label(localize "Parameters") 
		command("DISPLAY/APP=AARSYNC/FAM=%DISFAM%/VIEWPORT=AARSYNC_PARAMETER_POPUP AARSYNC_PARAMETER") 
		label(localize "Message Log") 
		command("DISPLAY/APP=AARSYNC/FAM=%DISFAM%/VIEWPORT=AARSYNC_LOG_POPUP AARSYNC_LOG") 
	);
	
	menu "AARSYNC_DISPLAY_DIRECTORY_OLD"
	(
		label(localize "Parameters Popup") 
		command("DISPLAY/VIEWPORT=VP_AARSYNC_PARAMETER_POPUP/APP=AARSYNC/FAM=%DISFAM% AARSYNC_PARAMETER_POPUP") 
		label(localize "Log Popup") 
		command("DISPLAY/VIEWPORT=VP_AARSYNC_LOG_POPUP/APP=AARSYNC/FAM=%DISFAM% AARSYNC_LOG_POPUP") 
	);
	
	menu "AARSYNC_MESSAGE_LOG_FILTER"
	(
		label(localize "Text Contains") 
		command("FILTER/LAYER=DEFAULT/NOHIER (TEXT_MESS:AARSYNC_LOG_DATA LIKE \\"%CMD%\\")") 
		label(localize "Info") 
		command("FILTER/LAYER=DEFAULT/NOHIER (SEV_MESS:AARSYNC_LOG_DATA EQ \\"I\\")") 
		label(localize "Warning") 
		command("FILTER/LAYER=DEFAULT/NOHIER (SEV_MESS:AARSYNC_LOG_DATA EQ \\"W\\")") 
		label(localize "Error") 
		command("FILTER/LAYER=DEFAULT/NOHIER (SEV_MESS:AARSYNC_LOG_DATA EQ \\"E\\")") 
		label(localize "Fatal") 
		command("FILTER/LAYER=DEFAULT/NOHIER (SEV_MESS:AARSYNC_LOG_DATA EQ \\"F\\")") 
	);
	
	menu "AARSYNC_SELECT_MANUAL"
	(
		label(localize "Set/Reset Manual") 
		command("MASK/TOGGLE=MAN/NOPOPUP/USER=%DISAPP%_%DISFAM%") 
	);
	
	picture "AARSYNC_AARELE_POPUP"
	(
		database("AARSDB") 
		record("AARELE") 
		
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(0 1) 
			show_char("A") 
			size(64) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("SPIMSG_AARELE") 
		)
	);
	
	picture "AARSYNC_APP_STATUS"
	(
		database("AARSDB") 
		record("ITEMS") 
		
		formatted_field 
		(
			gab "AARSYNC_HYPER_LINK" 
			keyset "AARSYNC_OPEN_LOG_POPUP" 
			origin(0 0) 
			tool_tip(localize "AARSYNC Application Status Message")
			show_char("A") 
			size(32) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("STATUS") 
		)
	);
	
	picture "AARSYNC_CB_DASHBOARD_DATA"
	(
		database("AARSDB") 
		record("AARELE") 
		
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			cam "AARSYNC_NORM_AARELE_VISIBLE" 
			origin(632 1) 
			show_char("3") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("NORM_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			origin(551 1) 
			show_char("2") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("OPER_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(85 1) 
			show_char("A") 
			size(14) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("ID_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(374 1) 
			show_char("F") 
			size(8) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("FRST_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(229 1) 
			show_char("S") 
			size(4) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("SEGID_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(452 1) 
			show_char("T") 
			size(8) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("FRST_AARELE") 
		)
		conditional_text "AARSYNC_TYPE_AARELE" 
		(
			origin(315 2) 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(14 1) 
			show_char("C") 
			size(6) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("COID_AARELE") 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_OUT" 
		(
			origin(800 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_OUT" 
		(
			origin(880 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_OUT" 
		(
			origin(959 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_IN" 
		(
			cam "AARSYNC_NETSUB_AARELE_GT0_VISIBLE" 
			origin(800 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_IN" 
		(
			cam "AARSYNC_SMAP_AARELE_TRUE_VISIBLE" 
			origin(880 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_IN" 
		(
			cam "AARSYNC_DNV_AARELE_TRUE_VISIBLE" 
			origin(959 2) 
		)
		symbol "AARSYNC_STD_ENTRY_BOX_60PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			cam "AARSYNC_MAN_AARELE_TRUE_VISIBLE" 
			origin(535 0) 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			cam "AARSYNC_MAN_AARELE_TRUE_VISIBLE" 
			origin(551 1) 
			tool_tip(localize "File Ratings %%FILEOPER_AARELE%%
User Modified the Ratings - %%USER_AARELE%%")
			show_char("2") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("OPER_AARELE") 
		)
		symbol "AARSYNC_STD_ENTRY_BOX_60PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			cam "AARSYNC_MAN_NORM_AARELE_VISIBLE" 
			origin(616 0) 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			cam "AARSYNC_MAN_NORM_AARELE_VISIBLE" 
			origin(632 1) 
			tool_tip(localize "File Ratings %%FILENORM_AARELE%%
User Modified the Ratings - %%USER_AARELE%%")
			show_char("3") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("NORM_AARELE") 
		)
	);
	
	picture "AARSYNC_DASHBOARD_HEADER"
	(
		database("AARSDB") 
		record("ITEMS") 
		
		rectangle 
		(
			gab "STD_PRIM_HEADER_BANNER_BACKGROUND" 
			origin(0 0) 
			width(1080)
			height(30)
		)
		polyline 
		(
			gab "STD_PRIM_HEADER_BANNER_DIVIDER_LINE" 
			origin(0 0) 
			point(0 0) 
			point(0 30) 
		)
		polyline 
		(
			gab "STD_PRIM_HEADER_BANNER_DIVIDER_LINE" 
			origin(600 0) 
			point(600 0) 
			point(0 30) 
		)
		polyline 
		(
			gab "STD_PRIM_HEADER_BANNER_DIVIDER_LINE" 
			origin(200 0) 
			point(200 0) 
			point(0 30) 
		)
		text 
		(
			gab "STD_TEXT_TITLE_COLUMN_HEADERS" 
			origin(456 0) 
			localize "   To
Station"
		)
		polyline 
		(
			gab "STD_PRIM_HEADER_BANNER_DIVIDER_LINE" 
			origin(360 0) 
			point(360 0) 
			point(0 30) 
		)
		polyline 
		(
			gab "STD_PRIM_HEADER_BANNER_DIVIDER_LINE" 
			origin(440 0) 
			point(440 0) 
			point(0 30) 
		)
		polyline 
		(
			gab "STD_PRIM_HEADER_BANNER_DIVIDER_LINE" 
			origin(520 0) 
			point(520 0) 
			point(0 30) 
		)
		text 
		(
			gab "AARSYNC_TEXT_COLUMN_HEADERS_SORT" 
			cam "AARSYNC_ID_AARELE_SORT_ASCENDING_TRUE" 
			keyset "AARSYNC_ID_AARELE_SORT_ASCENDING" 
			origin(102 7) 
			localize "Element ID"
		)
		polyline 
		(
			gab "STD_PRIM_HEADER_BANNER_DIVIDER_LINE" 
			origin(280 0) 
			point(280 0) 
			point(0 30) 
		)
		text 
		(
			gab "STD_TEXT_TITLE_COLUMN_HEADERS" 
			origin(377 0) 
			localize " From
Station"
		)
		text 
		(
			gab "STD_TEXT_TITLE_COLUMN_HEADERS" 
			origin(206 8) 
			localize "Segment ID"
		)
		text 
		(
			gab "AARSYNC_TEXT_COLUMN_HEADERS_SORT" 
			cam "AARSYNC_MAN_AARELE_KEY236_SORT_ASC_T" 
			keyset "AARSYNC_MAN_AARELE_KEY236_SORT_ASC" 
			origin(526 7) 
			localize "OPER Limit"
		)
		text 
		(
			gab "AARSYNC_TEXT_COLUMN_HEADERS_SORT" 
			keyset "AARSYNC_ID_AARELE_SORT_DESCENDING" 
			cam "AARSYNC_ID_AARELE_SORT_DESCENDING_TRUE" 
			origin(102 7) 
			localize "Element ID"
		)
		text 
		(
			gab "AARSYNC_TEXT_COLUMN_HEADERS_SORT" 
			cam "AARSYNC_TYPE_AARELE_SORT_ASCENDING_TRUE" 
			keyset "AARSYNC_TYPE_AARELE_SORT_ASCENDING" 
			origin(306 7) 
			localize "Type"
		)
		text 
		(
			gab "AARSYNC_TEXT_COLUMN_HEADERS_SORT" 
			cam "AARSYNC_TYPE_AARELE_SORT_DESCENDING_TRUE" 
			keyset "AARSYNC_TYPE_AARELE_SORT_DESCENDING" 
			origin(306 7) 
			localize "Type"
		)
		polyline 
		(
			gab "STD_PRIM_HEADER_BANNER_DIVIDER_LINE" 
			origin(70 0) 
			point(70 0) 
			point(0 30) 
		)
		text 
		(
			gab "STD_TEXT_TITLE_COLUMN_HEADERS" 
			origin(7 8) 
			localize "Company"
		)
		polyline 
		(
			gab "STD_PRIM_HEADER_BANNER_DIVIDER_LINE" 
			origin(680 0) 
			point(680 0) 
			point(0 30) 
		)
		polyline 
		(
			gab "STD_PRIM_HEADER_BANNER_DIVIDER_LINE" 
			origin(760 0) 
			point(760 0) 
			point(0 30) 
		)
		polyline 
		(
			gab "STD_PRIM_HEADER_BANNER_DIVIDER_LINE" 
			origin(840 0) 
			point(840 0) 
			point(0 30) 
		)
		polyline 
		(
			gab "STD_PRIM_HEADER_BANNER_DIVIDER_LINE" 
			origin(920 0) 
			point(920 0) 
			point(0 30) 
		)
		text 
		(
			gab "AARSYNC_TEXT_COLUMN_HEADERS_SORT" 
			cam "AARSYNC_NETSUB_AARELE_SORT_ASCENDING_TRUE" 
			keyset "AARSYNC_NETSUB_AARELE_SORT_ASCENDING" 
			origin(769 7) 
			tool_tip(localize "Is the Facility Mapped to NETMOM?")
			localize "NET Map?"
		)
		text 
		(
			gab "AARSYNC_TEXT_COLUMN_HEADERS_SORT" 
			cam "AARSYNC_SMAP_AARELE_SORT_ASCENDING_TRUE" 
			keyset "AARSYNC_SMAP_AARELE_SORT_ASCENDING" 
			origin(850 7) 
			tool_tip(localize "Is the Facility Mapped to SCADA?")
			localize "SCA Map?"
		)
		text 
		(
			gab "AARSYNC_TEXT_COLUMN_HEADERS_SORT" 
			cam "AARSYNC_DNV_AARELE_SORT_ASCENDING_TRUE" 
			keyset "AARSYNC_DNV_AARELE_SORT_ASCENDING" 
			origin(929 7) 
			tool_tip(localize "Is the Facility Mapped with DNV File?")
			localize "DNV Map?"
		)
		text 
		(
			gab "AARSYNC_TEXT_COLUMN_HEADERS_SORT" 
			cam "AARSYNC_NETSUB_AARELE_SORT_DESCENDING_TRUE" 
			keyset "AARSYNC_NETSUB_AARELE_SORT_DESCENDING" 
			origin(769 7) 
			tool_tip(localize "Is the Facility Mapped to NETMOM?")
			localize "NET Map?"
		)
		text 
		(
			gab "AARSYNC_TEXT_COLUMN_HEADERS_SORT" 
			cam "AARSYNC_SMAP_AARELE_SORT_DESCENDING_TRUE" 
			keyset "AARSYNC_SMAP_AARELE_SORT_DESCENDING" 
			origin(850 7) 
			tool_tip(localize "Is the Facility Mapped to SCADA?")
			localize "SCA Map?"
		)
		text 
		(
			gab "AARSYNC_TEXT_COLUMN_HEADERS_SORT" 
			cam "AARSYNC_DNV_AARELE_SORT_DESCENDING_TRUE" 
			keyset "AARSYNC_DNV_AARELE_SORT_DESCENDING" 
			origin(929 7) 
			tool_tip(localize "Is the Facility Mapped with DNV File?")
			localize "DNV Map?"
		)
		text 
		(
			gab "AARSYNC_TEXT_COLUMN_HEADERS_SORT" 
			cam "AARSYNC_MAN_AARELE_KEY236_SORT_DES_T" 
			keyset "AARSYNC_MAN_AARELE_KEY236_SORT_DES" 
			origin(526 7) 
			localize "OPER Limit"
		)
		text 
		(
			gab "AARSYNC_TEXT_COLUMN_HEADERS_SORT" 
			cam "AARSYNC_MAN_AARELE_KEY236_SORT_ASC_T" 
			keyset "AARSYNC_MAN_AARELE_KEY236_SORT_ASC" 
			origin(604 7) 
			localize "NORM Limit"
		)
		text 
		(
			gab "AARSYNC_TEXT_COLUMN_HEADERS_SORT" 
			cam "AARSYNC_MAN_AARELE_KEY236_SORT_DES_T" 
			keyset "AARSYNC_MAN_AARELE_KEY236_SORT_DES" 
			origin(604 7) 
			localize "NORM Limit"
		)
		text 
		(
			gab "AARSYNC_TEXT_COLUMN_HEADERS_SORT" 
			cam "AARSYNC_MAN_AARELE_KEY236_SORT_DES_T" 
			keyset "AARSYNC_MAN_AARELE_KEY236_SORT_DES" 
			origin(684 7) 
			localize "EMER Limit"
		)
		text 
		(
			gab "AARSYNC_TEXT_COLUMN_HEADERS_SORT" 
			cam "AARSYNC_MAN_AARELE_KEY236_SORT_ASC_T" 
			keyset "AARSYNC_MAN_AARELE_KEY236_SORT_ASC" 
			origin(684 7) 
			localize "EMER Limit"
		)
		polyline 
		(
			gab "STD_PRIM_HEADER_BANNER_DIVIDER_LINE" 
			origin(1000 0) 
			point(1000 0) 
			point(0 30) 
		)
		text 
		(
			gab "AARSYNC_TEXT_COLUMN_HEADERS_SORT" 
			cam "AARSYNC_MAN_AARELE_KEY237_SORT_ASC_T" 
			keyset "AARSYNC_MAN_AARELE_KEY237_SORT_ASC" 
			origin(1011 7) 
			tool_tip(localize "Shows Miscellaneous Items")
			localize "Misc Info"
		)
		text 
		(
			gab "AARSYNC_TEXT_COLUMN_HEADERS_SORT" 
			cam "AARSYNC_MAN_AARELE_KEY237_SORT_DES_T" 
			keyset "AARSYNC_MAN_AARELE_KEY237_SORT_DES" 
			origin(1011 7) 
			tool_tip(localize "Shows Miscellaneous Items")
			localize "Misc Info"
		)
	);
	
	picture "AARSYNC_GENERAL_PARAMETERS"
	(
		database("AARSDB") 
		record("ITEMS") 
		
		rectangle 
		(
			gab (
				foreground("113,111,100") 
			)
			origin(290 290) 
			width(270)
			height(160)
		)
		rectangle 
		(
			gab (
				foreground("113,111,100") 
			)
			origin(10 120) 
			width(270)
			height(160)
		)
		rectangle 
		(
			gab (
				foreground("113,111,100") 
			)
			origin(290 120) 
			width(270)
			height(160)
		)
		rectangle 
		(
			gab (
				foreground("113,111,100") 
			)
			origin(10 50) 
			width(550)
			height(60)
		)
		symbol "AARSYNC_STD_ENTRY_BOX_400PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			origin(130 81) 
			tool_tip(localize "DNV File name including Path (Can contain environment variables)
Refer to AARSYNC user guide for details.")
		)
		rectangle 
		(
			gab (
				foreground("113,111,100") 
			)
			origin(10 290) 
			width(270)
			height(160)
		)
		text 
		(
			gab "STD_TEXT_BACKGROUN_UNDERSCORE" 
			origin(60 130) 
			localize "Source Application Parameters"
		)
		text 
		(
			gab "STD_TEXT_BACKGROUN_UNDERSCORE" 
			origin(67 300) 
			localize "Debug Manager Parameters"
		)
		text 
		(
			gab "STD_TEXT_BACKGROUND" 
			origin(104 330) 
			localize "Debug Level"
		)
		symbol "AARSYNC_STD_ENTRY_BOX_80PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			origin(185 329) 
			tool_tip(localize "Debug Manager - Debug Message level (Default value = 0).
Refer to AARSYNC user guide for details.")
		)
		formatted_field 
		(
			gab "AARSYNC_FV_ENTRY" 
			origin(215 330) 
			show_char("2") 
			size(3) 
			position(3) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("DBGLVL_ITEMS") 
		)
		text 
		(
			gab "STD_TEXT_BACKGROUND" 
			origin(86 360) 
			localize "Debug Function"
		)
		symbol "AARSYNC_STD_ENTRY_BOX_80PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			origin(185 359) 
			tool_tip(localize "Debug Manager - Debug Function Level (Default = 0).
Refer to AARSYNC user guide for details.")
		)
		formatted_field 
		(
			gab "AARSYNC_FV_ENTRY" 
			origin(215 360) 
			show_char("3") 
			size(3) 
			position(3) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("DBGFUN_ITEMS") 
		)
		text 
		(
			gab "STD_TEXT_BACKGROUN_UNDERSCORE" 
			origin(375 128) 
			localize "Timer Parameters"
		)
		text 
		(
			gab "STD_TEXT_BACKGROUND" 
			origin(324 160) 
			localize "DNV File Old Threshold"
		)
		symbol "AARSYNC_STD_ENTRY_BOX_80PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			origin(465 159) 
			tool_tip(localize "When DNV File is older than these many seconds FILEOLD flag is set to true. (Default 2 Hours or 2*60*60 seconds)
Refer to AARSYNC user guide for details.")
		)
		formatted_field 
		(
			gab "AARSYNC_FV_ENTRY" 
			origin(484 160) 
			show_char("2") 
			size(6) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("FLOLDTH") 
		)
		text 
		(
			gab "STD_TEXT_BACKGROUND" 
			origin(59 160) 
			localize "NETMOM Application"
		)
		text 
		(
			gab "STD_TEXT_BACKGROUND" 
			origin(83 190) 
			localize "NETMOM Family"
		)
		symbol "AARSYNC_STD_ENTRY_BOX_80PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			origin(185 159) 
			tool_tip(localize "Source NETMOM Application Name (Default = \\"RTNET\\")
Refer to AARSYNC user guide for details.")
		)
		formatted_field 
		(
			gab "AARSYNC_FV_ENTRY" 
			origin(193 160) 
			show_char("A") 
			size(8) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("NETAPP_ITEMS") 
		)
		symbol "AARSYNC_STD_ENTRY_BOX_80PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			origin(185 189) 
			tool_tip(localize "Source NETMOM Family Name (Default = \\"EMS\\")
Refer to AARSYNC user guide for details.")
		)
		formatted_field 
		(
			gab "AARSYNC_FV_ENTRY" 
			origin(193 190) 
			show_char("A") 
			size(8) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("NETFAM_ITEMS") 
		)
		text 
		(
			gab "STD_TEXT_BACKGROUND" 
			origin(43 220) 
			localize "SCADAMOM Application"
		)
		text 
		(
			gab "STD_TEXT_BACKGROUND" 
			origin(67 250) 
			localize "SCADAMOM Family"
		)
		symbol "AARSYNC_STD_ENTRY_BOX_80PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			origin(185 219) 
			tool_tip(localize "Source SCADAMOM Application Name (Default = \\"SCADA\\")
Refer to AARSYNC user guide for details.")
		)
		formatted_field 
		(
			gab "AARSYNC_FV_ENTRY" 
			origin(193 220) 
			show_char("A") 
			size(8) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("SCAPP_ITEMS") 
		)
		symbol "AARSYNC_STD_ENTRY_BOX_80PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			origin(185 249) 
			tool_tip(localize "Source SCADAMOM Family Name (Default = \\"EMS\\")
Refer to AARSYNC user guide for details.")
		)
		formatted_field 
		(
			gab "AARSYNC_FV_ENTRY" 
			origin(193 250) 
			show_char("A") 
			size(8) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("SCFAM_ITEMS") 
		)
		text 
		(
			gab "STD_TEXT_BACKGROUN_UNDERSCORE" 
			origin(364 298) 
			localize "Monitored Parameters"
		)
		text 
		(
			gab "STD_TEXT_BACKGROUND" 
			origin(305 330) 
			localize "NETMOM Val Time"
		)
		text 
		(
			gab "STD_TEXT_BACKGROUND" 
			origin(30 390) 
			localize "Max Number of Messages"
		)
		symbol "AARSYNC_STD_ENTRY_BOX_80PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			origin(185 389) 
			tool_tip(localize "Maximum number of messages written to debug file (Default = 100,000).
Refer to AARSYNC user guide for details.")
		)
		formatted_field 
		(
			gab "AARSYNC_FV_ENTRY" 
			origin(201 390) 
			show_char("4") 
			size(7) 
			position(3) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("MAXMSG_ITEMS") 
		)
		text 
		(
			gab "AARSYNC_TEXT_TITLE" 
			origin(10 15) 
			localize "AARSYNC Parameters"
		)
		text 
		(
			gab "STD_TEXT_BACKGROUND" 
			origin(50 420) 
			localize "Messages to STDOUT"
		)
		symbol "AARSYNC_BTN_TOGGLE_OUT" 
		(
			keyset "AARSYNC_TOGGLE_STDOUT" 
			origin(185 421) 
			tool_tip(localize "Export Debug Messages to STDOUT (Default = Unchecked).
Refer to AARSYNC user guide for details.")
		)
		symbol "AARSYNC_BTN_TOGGLE_IN" 
		(
			cam "AARSYNC_VISABLE_IF_STDOUT_TRUE" 
			keyset "AARSYNC_TOGGLE_STDOUT" 
			origin(185 421) 
			tool_tip(localize "Export Debug Messages to STDOUT (Default = Unchecked).
Refer to AARSYNC user guide for details.")
		)
		text 
		(
			gab "STD_TEXT_BACKGROUND" 
			origin(26 82) 
			localize "DNV Ratings File"
		)
		text 
		(
			gab "STD_TEXT_BACKGROUN_UNDERSCORE" 
			origin(227 60) 
			localize "DNV File Parameters"
		)
		formatted_field 
		(
			gab "AARSYNC_FV_ENTRY" 
			origin(136 83) 
			show_char("I") 
			size(100) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("DNVFILE_ITEMS") 
		)
		text 
		(
			gab "STD_TEXT_BACKGROUND" 
			origin(302 190) 
			localize "SCAPI Table Refresh Timer"
		)
		symbol "AARSYNC_STD_ENTRY_BOX_80PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			origin(465 189) 
			tool_tip(localize "SCAPI Update Set Refresh Time (default = 10 seconds)
Refer to AARSYNC user guide for details.")
		)
		formatted_field 
		(
			gab "AARSYNC_FV_ENTRY" 
			origin(484 190) 
			show_char("3") 
			size(6) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("SPIURTE") 
		)
		text 
		(
			gab "STD_TEXT_BACKGROUND" 
			origin(313 220) 
			localize "Periodic Timer Frequency"
		)
		symbol "AARSYNC_STD_ENTRY_BOX_80PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			origin(465 219) 
			tool_tip(localize "Periodic timer in seconds to check file updates (default = 5*60)
Refer to AARSYNC user guide for details.")
		)
		formatted_field 
		(
			gab "AARSYNC_FV_ENTRY" 
			origin(484 220) 
			show_char("4") 
			size(6) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("PERTM") 
		)
		text 
		(
			gab "STD_TEXT_BACKGROUND" 
			origin(338 250) 
			localize "Periodic Timer Offset"
		)
		symbol "AARSYNC_STD_ENTRY_BOX_80PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			origin(465 249) 
			tool_tip(localize "Periodic timer offset in seconds (default = 0)
Refer to AARSYNC user guide for details.")
		)
		formatted_field 
		(
			gab "AARSYNC_FV_ENTRY" 
			origin(484 250) 
			show_char("5") 
			size(6) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("OFFTM") 
		)
		text 
		(
			gab "STD_TEXT_BACKGROUND" 
			origin(316 360) 
			localize "SCADA Val Time"
		)
		text 
		(
			gab "STD_TEXT_BACKGROUND" 
			origin(303 390) 
			localize "SCADA Read Time"
		)
		text 
		(
			gab "STD_TEXT_BACKGROUND" 
			origin(295 420) 
			localize "DNV File Read Time"
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(416 330) 
			tool_tip(localize "NETMOM Validation time as seen by AARSync")
			show_char("T") 
			size(20) 
			position(1) 
			format(timedate(time_and_date)) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("TVALNMD") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(416 360) 
			tool_tip(localize "SCADAMOM Validation time as seen by AARSync")
			show_char("T") 
			size(20) 
			position(1) 
			format(timedate(time_and_date)) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("VFYMOM") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(416 390) 
			tool_tip(localize "Last SCADAMOM read time to register with SCAPI")
			show_char("T") 
			size(20) 
			position(1) 
			format(timedate(time_and_date)) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("SCADART") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(416 420) 
			tool_tip(localize "Last DNV file read time as seen by AARSync. (Red when old)")
			show_char("T") 
			size(20) 
			position(1) 
			format(timedate(time_and_date)) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("FILERT") 
		)
		width(1150)
		height(950)
	);
	
	picture "AARSYNC_LINE_DASHBOARD_DATA"
	(
		database("AARSDB") 
		record("AARELE") 
		
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			cam "AARSYNC_NORM_AARELE_VISIBLE" 
			origin(632 1) 
			show_char("3") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("NORM_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			origin(551 1) 
			show_char("2") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("OPER_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(85 1) 
			show_char("A") 
			size(14) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("ID_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(374 1) 
			show_char("F") 
			size(8) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("FRST_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(229 1) 
			show_char("S") 
			size(4) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("SEGID_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(452 1) 
			show_char("T") 
			size(8) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("FRST_AARELE") 
		)
		conditional_text "AARSYNC_TYPE_AARELE" 
		(
			origin(315 2) 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(14 1) 
			show_char("C") 
			size(6) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("COID_AARELE") 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_OUT" 
		(
			origin(800 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_OUT" 
		(
			origin(880 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_OUT" 
		(
			origin(959 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_IN" 
		(
			cam "AARSYNC_NETSUB_AARELE_GT0_VISIBLE" 
			origin(800 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_IN" 
		(
			cam "AARSYNC_SMAP_AARELE_TRUE_VISIBLE" 
			origin(880 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_IN" 
		(
			cam "AARSYNC_DNV_AARELE_TRUE_VISIBLE" 
			origin(959 2) 
		)
		symbol "AARSYNC_STD_ENTRY_BOX_60PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			cam "AARSYNC_MAN_AARELE_TRUE_VISIBLE" 
			origin(535 0) 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			cam "AARSYNC_MAN_AARELE_TRUE_VISIBLE" 
			origin(551 1) 
			tool_tip(localize "File Ratings %%FILEOPER_AARELE%%
User Modified the Ratings - %%USER_AARELE%%")
			show_char("2") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("OPER_AARELE") 
		)
		symbol "AARSYNC_STD_ENTRY_BOX_60PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			cam "AARSYNC_MAN_NORM_AARELE_VISIBLE" 
			origin(616 0) 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			cam "AARSYNC_MAN_NORM_AARELE_VISIBLE" 
			origin(632 1) 
			tool_tip(localize "File Ratings %%FILENORM_AARELE%%
User Modified the Ratings - %%USER_AARELE%%")
			show_char("3") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("NORM_AARELE") 
		)
	);
	
	picture "AARSYNC_LN_DASHBOARD_DATA"
	(
		database("AARSDB") 
		record("AARELE") 
		
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			cam "AARSYNC_NORM_AARELE_VISIBLE" 
			origin(632 1) 
			show_char("3") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("NORM_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			origin(551 1) 
			show_char("2") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("OPER_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(85 1) 
			show_char("A") 
			size(14) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("ID_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(374 1) 
			show_char("F") 
			size(8) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("FRST_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(229 1) 
			show_char("S") 
			size(4) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("SEGID_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(452 1) 
			show_char("T") 
			size(8) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("FRST_AARELE") 
		)
		conditional_text "AARSYNC_TYPE_AARELE" 
		(
			origin(315 2) 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(14 1) 
			show_char("C") 
			size(6) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("COID_AARELE") 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_OUT" 
		(
			origin(800 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_OUT" 
		(
			origin(880 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_OUT" 
		(
			origin(959 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_IN" 
		(
			cam "AARSYNC_NETSUB_AARELE_GT0_VISIBLE" 
			origin(800 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_IN" 
		(
			cam "AARSYNC_SMAP_AARELE_TRUE_VISIBLE" 
			origin(880 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_IN" 
		(
			cam "AARSYNC_DNV_AARELE_TRUE_VISIBLE" 
			origin(959 2) 
		)
		symbol "AARSYNC_STD_ENTRY_BOX_60PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			cam "AARSYNC_MAN_AARELE_TRUE_VISIBLE" 
			origin(535 0) 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			cam "AARSYNC_MAN_AARELE_TRUE_VISIBLE" 
			origin(551 1) 
			tool_tip(localize "File Ratings %%FILEOPER_AARELE%%
User Modified the Ratings - %%USER_AARELE%%")
			show_char("2") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("OPER_AARELE") 
		)
		symbol "AARSYNC_STD_ENTRY_BOX_60PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			cam "AARSYNC_MAN_NORM_AARELE_VISIBLE" 
			origin(616 0) 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			cam "AARSYNC_MAN_NORM_AARELE_VISIBLE" 
			origin(632 1) 
			tool_tip(localize "File Ratings %%FILENORM_AARELE%%
User Modified the Ratings - %%USER_AARELE%%")
			show_char("3") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("NORM_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			cam "AARSYNC_EMER_AARELE_VISIBLE" 
			origin(710 1) 
			show_char("4") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("EMER_AARELE") 
		)
		symbol "AARSYNC_STD_ENTRY_BOX_60PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			cam "AARSYNC_MAN_EMER_AARELE_VISIBLE" 
			origin(694 0) 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			cam "AARSYNC_MAN_EMER_AARELE_VISIBLE" 
			origin(710 1) 
			tool_tip(localize "File Ratings %%FILEEMER_AARELE%%
User Modified the Ratings - %%USER_AARELE%%")
			show_char("4") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("EMER_AARELE") 
		)
	);
	
	picture "AARSYNC_LOG_DATA"
	(
		database("AARSLG") 
		record("MESS") 
		
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(0 1) 
			show_char("T") 
			size(20) 
			position(1) 
			format(timedate(time_and_date)) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("TIME_MESS") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			cam "AARSYNC_SEV_MESSAGE_COLOR" 
			origin(166 1) 
			show_char("A") 
			size(128) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("TEXT_MESS") 
		)
	);
	
	picture "AARSYNC_LOG_HEADER"
	(
		database("AARSLG") 
		record("ITEMS") 
		
		rectangle 
		(
			gab "STD_PRIM_HEADER_BANNER_BACKGROUND" 
			origin(0 0) 
			width(1045)
			height(30)
		)
		text 
		(
			gab "STD_TEXT_TITLE_COLUMN_HEADERS" 
			origin(64 8) 
			localize "Time"
		)
		text 
		(
			gab "STD_TEXT_TITLE_COLUMN_HEADERS" 
			origin(260 8) 
			localize "Message"
		)
		polyline 
		(
			gab "STD_PRIM_HEADER_BANNER_DIVIDER_LINE" 
			origin(160 0) 
			point(160 0) 
			point(0 30) 
		)
		width(2000)
		height(30)
	);
	
	picture "AARSYNC_MISC_DASHBOARD_DATA"
	(
		database("AARSDB") 
		record("AARELE") 
		
		text 
		(
			
			cam "AARSYNC_SCAPI_ERROR" gab "AARSYNC_ALARM_TEXT" 
			picture "AARSYNC_AARELE_POPUP" 
			(
				 origin(0 0) 
				
			)
			
			origin(0 0) 
			tool_tip(localize "Scapi Write Error - %%SPIMSG_AARELE%%")
			localize "E"
		)
	);
	
	picture "AARSYNC_XF_DASHBOARD_DATA"
	(
		database("AARSDB") 
		record("AARELE") 
		
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			origin(551 1) 
			show_char("2") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("OPER_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(85 1) 
			show_char("A") 
			size(14) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("ID_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(374 1) 
			show_char("F") 
			size(8) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("FRST_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(229 1) 
			show_char("S") 
			size(4) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("SEGID_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(452 1) 
			show_char("T") 
			size(8) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("FRST_AARELE") 
		)
		conditional_text "AARSYNC_TYPE_AARELE" 
		(
			origin(315 2) 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(14 1) 
			show_char("C") 
			size(6) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("COID_AARELE") 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_OUT" 
		(
			origin(800 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_OUT" 
		(
			origin(880 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_OUT" 
		(
			origin(959 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_IN" 
		(
			cam "AARSYNC_NETSUB_AARELE_GT0_VISIBLE" 
			origin(800 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_IN" 
		(
			cam "AARSYNC_SMAP_AARELE_TRUE_VISIBLE" 
			origin(880 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_IN" 
		(
			cam "AARSYNC_DNV_AARELE_TRUE_VISIBLE" 
			origin(959 2) 
		)
		symbol "AARSYNC_STD_ENTRY_BOX_60PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			cam "AARSYNC_MAN_AARELE_TRUE_VISIBLE" 
			origin(535 0) 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			cam "AARSYNC_MAN_AARELE_TRUE_VISIBLE" 
			origin(551 1) 
			tool_tip(localize "File Ratings %%FILEOPER_AARELE%%
User Modified the Ratings - %%USER_AARELE%%")
			show_char("2") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("OPER_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			cam "AARSYNC_EMER_AARELE_VISIBLE" 
			origin(710 1) 
			show_char("4") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("EMER_AARELE") 
		)
		symbol "AARSYNC_STD_ENTRY_BOX_60PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			cam "AARSYNC_MAN_EMER_AARELE_VISIBLE" 
			origin(694 0) 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			cam "AARSYNC_MAN_EMER_AARELE_VISIBLE" 
			origin(710 1) 
			tool_tip(localize "File Ratings %%FILEEMER_AARELE%%
User Modified the Ratings - %%USER_AARELE%%")
			show_char("4") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("EMER_AARELE") 
		)
	);
	
	picture "AARSYNC_ZB_DASHBOARD_DATA"
	(
		database("AARSDB") 
		record("AARELE") 
		
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			cam "AARSYNC_NORM_AARELE_VISIBLE" 
			origin(632 1) 
			show_char("3") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("NORM_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			origin(551 1) 
			show_char("2") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("OPER_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(85 1) 
			show_char("A") 
			size(14) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("ID_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(374 1) 
			show_char("F") 
			size(8) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("FRST_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(229 1) 
			show_char("S") 
			size(4) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("SEGID_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(452 1) 
			show_char("T") 
			size(8) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("FRST_AARELE") 
		)
		conditional_text "AARSYNC_TYPE_AARELE" 
		(
			origin(315 2) 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			origin(14 1) 
			show_char("C") 
			size(6) 
			position(1) 
			format(char) 
			justification(left) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("COID_AARELE") 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_OUT" 
		(
			origin(800 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_OUT" 
		(
			origin(880 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_OUT" 
		(
			origin(959 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_IN" 
		(
			cam "AARSYNC_NETSUB_AARELE_GT0_VISIBLE" 
			origin(800 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_IN" 
		(
			cam "AARSYNC_SMAP_AARELE_TRUE_VISIBLE" 
			origin(880 2) 
		)
		symbol "AARSYNC_BTN_TOGGLE_NONENTER_IN" 
		(
			cam "AARSYNC_DNV_AARELE_TRUE_VISIBLE" 
			origin(959 2) 
		)
		symbol "AARSYNC_STD_ENTRY_BOX_60PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			cam "AARSYNC_MAN_AARELE_TRUE_VISIBLE" 
			origin(535 0) 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			cam "AARSYNC_MAN_AARELE_TRUE_VISIBLE" 
			origin(551 1) 
			tool_tip(localize "File Ratings %%FILEOPER_AARELE%%
User Modified the Ratings - %%USER_AARELE%%")
			show_char("2") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("OPER_AARELE") 
		)
		symbol "AARSYNC_STD_ENTRY_BOX_60PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			cam "AARSYNC_MAN_NORM_AARELE_VISIBLE" 
			origin(616 0) 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			cam "AARSYNC_MAN_NORM_AARELE_VISIBLE" 
			origin(632 1) 
			tool_tip(localize "File Ratings %%FILENORM_AARELE%%
User Modified the Ratings - %%USER_AARELE%%")
			show_char("3") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("NORM_AARELE") 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			cam "AARSYNC_EMER_AARELE_VISIBLE" 
			origin(710 1) 
			show_char("4") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			not enterable 
			show 
			field("EMER_AARELE") 
		)
		symbol "AARSYNC_STD_ENTRY_BOX_60PIX" 
		(
			keyset "AARSYNC_EDEC_ENTER" 
			cam "AARSYNC_MAN_EMER_AARELE_VISIBLE" 
			origin(694 0) 
		)
		formatted_field 
		(
			gab "STD_TEXT_FV" 
			keyset "RIGHTCLICK" 
			menu "AARSYNC_SELECT_MANUAL" 
			cam "AARSYNC_MAN_EMER_AARELE_VISIBLE" 
			origin(710 1) 
			tool_tip(localize "File Ratings %%FILEEMER_AARELE%%
User Modified the Ratings - %%USER_AARELE%%")
			show_char("4") 
			size(4) 
			position(1) 
			format(int)
			factor(1.0000000) 
			justification(right) 
			refresh_field 
			not refresh_indirect 
			enterable 
			show 
			not uppercase 
			field("EMER_AARELE") 
		)
	);
	
	conditional_text "AARSYNC_TYPE_AARELE"
	(
		record("AARELE") 
		
		datasource("default")gab "STD_TEXT_FV" 
		text(localize "N/A") 
		tool_tip(localize "Unknown Type") 
		if field("LINE_AARELE") true 
			gab() 
			text(localize "LINE") 
			tool_tip(localize "Line Type") 
		else if field("LN_AARELE") true 
			gab 
			(
				visible 
			)
			text(localize "LN") 
			tool_tip(localize "Line Segment") 
		else if field("ZB_AARELE") true 
			gab 
			(
				not visible 
			)
			text(localize "ZB") 
			tool_tip(localize "Zero Impedance Branch") 
		else if field("XF_AARELE") true 
			gab() 
			text(localize "XF") 
			tool_tip(localize "Transformer") 
		else if field("CB_AARELE") true 
			gab() 
			text(localize "CB") 
			tool_tip(localize "Switch") 
		else 
			gab() 
			text(localize "N/A") 
			tool_tip(localize "Unknown Type") 
	);
	
	symbol "AARSYNC_BTN_TOGGLE_IN"
	(
		rectangle 
		(
			gab "AARSYNC_STD_ENTRY_BOX_FRAME" 
			origin(1 1) 
			width(10)
			height(10)
		)
		polygon 
		(
			gab "AARSYNC_STD_BTN_TOGGLE_SHADOW" 
			origin(0 0) 
			point(1 11) 
			point(-1 1) 
			point(0 -12) 
			point(12 0) 
			point(-1 1) 
			point(-10 0) 
			point(0 10) 
		)
		polygon 
		(
			gab "AARSYNC_STD_BTN_TOGGLE_HIGHLIGHT" 
			origin(0 0) 
			point(0 12) 
			point(1 -1) 
			point(10 0) 
			point(0 -10) 
			point(1 -1) 
			point(0 12) 
			point(-12 0) 
		)
		polygon 
		(
			gab "AARSYNC_BTN_TOGGLE_CHECK_MARK" 
			origin(2 2) 
			point(2 7) 
			point(3 3) 
			point(5 -5) 
			point(0 -3) 
			point(-5 5) 
			point(-2 -2) 
			point(-1 2) 
		)
	);
	
	symbol "AARSYNC_BTN_TOGGLE_NONENTER_IN"
	(
		rectangle 
		(
			gab "AARSYNC_TOGGLE_NONENTER_FRONT" 
			origin(1 1) 
			width(10)
			height(10)
		)
		polygon 
		(
			gab "AARSYNC_TOGGLE_NONENTER_HIGHLIGHT" 
			origin(0 0) 
			point(1 11) 
			point(-1 1) 
			point(0 -12) 
			point(12 0) 
			point(-1 1) 
			point(-10 0) 
			point(0 10) 
		)
		polygon 
		(
			gab "AARSYNC_TOGGLE_NONENTER_SHADOW" 
			origin(0 0) 
			point(0 12) 
			point(1 -1) 
			point(10 0) 
			point(0 -10) 
			point(1 -1) 
			point(0 12) 
			point(-12 0) 
		)
		polygon 
		(
			gab "AARSYNC_TOGGLE_CHECK_MARK_NONENTRY" 
			origin(2 2) 
			point(2 7) 
			point(3 3) 
			point(5 -5) 
			point(0 -3) 
			point(-5 5) 
			point(-2 -2) 
			point(-1 2) 
		)
	);
	
	symbol "AARSYNC_BTN_TOGGLE_NONENTER_OUT"
	(
		rectangle 
		(
			gab "AARSYNC_TOGGLE_NONENTER_FRONT" 
			origin(1 1) 
			width(10)
			height(10)
		)
		polygon 
		(
			gab "AARSYNC_TOGGLE_NONENTER_HIGHLIGHT" 
			origin(0 0) 
			point(1 11) 
			point(-1 1) 
			point(0 -12) 
			point(12 0) 
			point(-1 1) 
			point(-10 0) 
			point(0 10) 
		)
		polygon 
		(
			gab "AARSYNC_TOGGLE_NONENTER_SHADOW" 
			origin(0 0) 
			point(0 12) 
			point(1 -1) 
			point(10 0) 
			point(0 -10) 
			point(1 -1) 
			point(0 12) 
			point(-12 0) 
		)
	);
	
	symbol "AARSYNC_BTN_TOGGLE_OUT"
	(
		rectangle 
		(
			gab "STD_BTN_TOGGLE_FRONT" 
			origin(1 1) 
			width(10)
			height(10)
		)
		polygon 
		(
			gab "AARSYNC_STD_BTN_TOGGLE_HIGHLIGHT" 
			origin(0 0) 
			point(1 11) 
			point(-1 1) 
			point(0 -12) 
			point(12 0) 
			point(-1 1) 
			point(-10 0) 
			point(0 10) 
		)
		polygon 
		(
			gab "AARSYNC_STD_BTN_TOGGLE_SHADOW" 
			origin(0 0) 
			point(0 12) 
			point(1 -1) 
			point(10 0) 
			point(0 -10) 
			point(1 -1) 
			point(0 12) 
			point(-12 0) 
		)
	);
	
	symbol "AARSYNC_BTN_TOGGLE_REMOVE"
	(
		rectangle 
		(
			gab "AARSYNC_ENTRY_BOX_OUTLINE" 
			origin(0 0) 
			width(12)
			height(12)
		)
		rectangle 
		(
			gab "AARSYNC_STD_ENTRY_BOX_FRAME" 
			origin(1 1) 
			width(10)
			height(10)
		)
		polygon 
		(
			gab "AARSYNC_BTN_TOGGLE_UNCHECK_MARK" 
			origin(2 2) 
			point(3 2) 
			point(-1 1) 
			point(3 3) 
			point(-3 3) 
			point(1 1) 
			point(3 -3) 
			point(3 3) 
			point(1 -1) 
			point(-3 -3) 
			point(3 -3) 
			point(-1 -1) 
			point(-3 3) 
			point(-3 -3) 
		)
		image("AARSYNC_Picture_Background.jpg") 
		image_attachment(display) 
	);
	
	symbol "AARSYNC_DIRECTORY_ICON"
	(
		polygon 
		(
			gab (
				foreground("240,230,140") 
				font_family("Courier New") 
				fill 
			)
			origin(1 1) 
			point(1 8) 
			point(0 -6) 
			point(1 -1) 
			point(3 0) 
			point(1 1) 
			point(4 0) 
			point(1 1) 
			point(0 5) 
			point(-10 0) 
		)
		polygon 
		(
			gab (
				foreground("113,111,100") 
				background("0,0,0") 
				font_family("Courier New") 
				line_width(1) 
			)
			origin(1 1) 
			point(1 8) 
			point(0 -6) 
			point(1 -1) 
			point(3 0) 
			point(1 1) 
			point(4 0) 
			point(1 1) 
			point(0 5) 
			point(-10 0) 
		)
		polygon 
		(
			gab (
				foreground("240,230,140") 
				font_family("Courier New") 
				fill 
			)
			origin(19 1) 
			point(19 8) 
			point(0 -6) 
			point(1 -1) 
			point(3 0) 
			point(1 1) 
			point(4 0) 
			point(1 1) 
			point(0 5) 
			point(-10 0) 
		)
		polygon 
		(
			gab (
				foreground("113,111,100") 
				background("0,0,0") 
				font_family("Courier New") 
				line_width(1) 
			)
			origin(19 1) 
			point(19 8) 
			point(0 -6) 
			point(1 -1) 
			point(3 0) 
			point(1 1) 
			point(4 0) 
			point(1 1) 
			point(0 5) 
			point(-10 0) 
		)
		polygon 
		(
			gab (
				foreground("240,230,140") 
				font_family("Courier New") 
				fill 
			)
			origin(19 10) 
			point(19 17) 
			point(0 -6) 
			point(1 -1) 
			point(3 0) 
			point(1 1) 
			point(4 0) 
			point(1 1) 
			point(0 5) 
			point(-10 0) 
		)
		polygon 
		(
			gab (
				foreground("113,111,100") 
				background("0,0,0") 
				font_family("Courier New") 
				line_width(1) 
			)
			origin(19 10) 
			point(19 17) 
			point(0 -6) 
			point(1 -1) 
			point(3 0) 
			point(1 1) 
			point(4 0) 
			point(1 1) 
			point(0 5) 
			point(-10 0) 
		)
		polyline 
		(
			gab (
				foreground("113,111,100") 
				background("0,0,0") 
				font_family("Courier New") 
				line_width(2) 
			)
			origin(16 4) 
			point(16 4) 
			point(0 12) 
		)
		polyline 
		(
			gab (
				foreground("113,111,100") 
				background("0,0,0") 
				font_family("Courier New") 
				line_width(2) 
			)
			origin(11 5) 
			point(11 5) 
			point(9 0) 
		)
		polyline 
		(
			gab (
				foreground("113,111,100") 
				background("0,0,0") 
				font_family("Courier New") 
				line_width(2) 
			)
			origin(17 15) 
			point(19 15) 
			point(-2 0) 
		)
		polyline 
		(
			gab (
				font_family("Courier New") 
				line_width(1) 
			)
			origin(20 2) 
			point(20 7) 
			point(0 -5) 
			point(3 0) 
		)
		polyline 
		(
			gab (
				font_family("Courier New") 
				line_width(1) 
			)
			origin(20 11) 
			point(20 16) 
			point(0 -5) 
			point(3 0) 
		)
		polyline 
		(
			gab (
				font_family("Courier New") 
				line_width(1) 
			)
			origin(2 2) 
			point(2 7) 
			point(0 -5) 
			point(3 0) 
		)
		polygon 
		(
			gab (
				foreground("210,180,140") 
				font_family("Courier New") 
				fill 
			)
			origin(2 4) 
			point(2 8) 
			point(4 -4) 
			point(9 0) 
			point(-4 4) 
			point(-9 0) 
		)
		polygon 
		(
			gab (
				foreground("113,111,100") 
				background("0,0,0") 
				font_family("Courier New") 
				line_width(1) 
			)
			origin(2 4) 
			point(2 8) 
			point(4 -4) 
			point(9 0) 
			point(-4 4) 
			point(-9 0) 
		)
	);
	
	symbol "AARSYNC_ICON_FILTER"
	(
		polygon 
		(
			gab (
				foreground("204,200,184") 
				background("0,0,0") 
				font_family("Courier New") 
				halo_style(pixel) 
				fill 
			)
			origin(3 4) 
			point(19 4) 
			point(-6 7) 
			point(-1 7) 
			point(-2 0) 
			point(-1 -6) 
			point(-6 -8) 
		)
		ellipse 
		(
			gab (
				foreground("113,111,100") 
				background("0,0,0") 
				font_family("Courier New") 
				line_width(1) 
				fill 
			)
			origin(0 0) 
			width(22)
			height(6)
		)
		polygon 
		(
			gab (
				foreground("255,255,255") 
				font_family("Courier New") 
				fill 
			)
			origin(5 6) 
			point(5 6) 
			point(5 6) 
			point(-2 -5) 
		)
		polygon 
		(
			gab (
				foreground("255,255,255") 
				font_family("Courier New") 
				fill 
			)
			origin(14 1) 
			point(15 1) 
			point(3 1) 
			point(-3 3) 
			point(-1 0) 
		)
	);
	
	symbol "AARSYNC_ICON_FILTER_CANCEL"
	(
		polygon 
		(
			gab (
				foreground("204,200,184") 
				background("0,0,0") 
				font_family("Courier New") 
				halo_style(pixel) 
				fill 
			)
			origin(3 4) 
			point(19 4) 
			point(-6 7) 
			point(-1 7) 
			point(-2 0) 
			point(-1 -6) 
			point(-6 -8) 
		)
		ellipse 
		(
			gab (
				foreground("113,111,100") 
				background("0,0,0") 
				font_family("Courier New") 
				line_width(1) 
				fill 
			)
			origin(0 0) 
			width(22)
			height(6)
		)
		polygon 
		(
			gab (
				foreground("255,255,255") 
				font_family("Courier New") 
				fill 
			)
			origin(5 6) 
			point(5 6) 
			point(5 6) 
			point(-2 -5) 
		)
		polygon 
		(
			gab (
				foreground("255,255,255") 
				font_family("Courier New") 
				fill 
			)
			origin(14 1) 
			point(15 1) 
			point(3 1) 
			point(-3 3) 
			point(-1 0) 
		)
		polyline 
		(
			gab (
				foreground("255,0,0") 
				font_family("Courier New") 
				line_width(2) 
			)
			origin(4 2) 
			point(4 16) 
			point(14 -14) 
		)
		circle 
		(
			gab (
				foreground("255,0,0") 
				font_family("Courier New") 
				line_width(2) 
			)
			origin(1 0) 
			diameter(20) 
		)
	);
	
	symbol "AARSYNC_ICON_LEGEND"
	(
		rectangle 
		(
			gab (
				foreground("218,165,32") 
				background("0,0,0") 
				font_family("Courier New") 
				halo_style(pixel) 
				fill 
			)
			origin(13 6) 
			width(1)
			height(2)
		)
		rectangle 
		(
			gab (
				foreground("218,165,32") 
				background("0,0,0") 
				font_family("Courier New") 
				halo_style(pixel) 
				fill 
			)
			origin(17 6) 
			width(1)
			height(2)
		)
		rectangle 
		(
			gab (
				foreground("218,165,32") 
				background("0,0,0") 
				font_family("Courier New") 
				halo_style(pixel) 
				fill 
			)
			origin(9 4) 
			width(9)
			height(1)
		)
		circle 
		(
			gab (
				foreground("218,165,32") 
				background("0,0,0") 
				font_family("Courier New") 
				line_width(2) 
				halo_style(pixel) 
			)
			origin(2 2) 
			diameter(6) 
		)
		circle 
		(
			gab (
				foreground("255,255,0") 
				font_family("Courier New") 
				fill 
			)
			origin(0 0) 
			diameter(4) 
		)
	);
	
	symbol "AARSYNC_ROW_SELECTED"
	(
		polygon 
		(
			gab (
				foreground("0,255,0") 
				fill 
			)
			origin(0 0) 
			point(5 0) 
			point(-5 5) 
			point(5 5) 
			point(5 -5) 
			point(-5 -5) 
		)
	);
	
	symbol "AARSYNC_STD_ENTRY_BOX_400PIX"
	(
		rectangle 
		(
			gab "STD_ENTRY_BOX_FRONT" 
			origin(0 0) 
			width(402)
			height(17)
		)
		rectangle 
		(
			gab "AARSYNC_STD_ENTRY_BOX_FRAME" 
			origin(1 1) 
			width(400)
			height(15)
		)
	);
	
	symbol "AARSYNC_STD_ENTRY_BOX_60PIX"
	(
		rectangle 
		(
			gab "AARSYNC_ENTRY_BOX_OUTLINE" 
			origin(0 0) 
			width(60)
			height(17)
		)
		rectangle 
		(
			gab "AARSYNC_STD_ENTRY_BOX_FRAME" 
			origin(1 1) 
			width(58)
			height(15)
		)
	);
	
	symbol "AARSYNC_STD_ENTRY_BOX_80PIX"
	(
		rectangle 
		(
			gab "AARSYNC_ENTRY_BOX_OUTLINE" 
			origin(0 0) 
			width(80)
			height(17)
		)
		rectangle 
		(
			gab "AARSYNC_STD_ENTRY_BOX_FRAME" 
			origin(1 1) 
			width(78)
			height(15)
		)
	);
	
	symbol "STD_BTN_POKE_60PIX"
	(
		rectangle 
		(
			gab "STD_BTN_POKE_FRONT" 
			origin(2 2) 
			width(56)
			height(18)
		)
		polygon 
		(
			gab "STD_BTN_POKE_SHADOW" 
			origin(0 0) 
			point(0 22) 
			point(2 -2) 
			point(56 0) 
			point(0 -18) 
			point(2 -2) 
			point(0 22) 
			point(-60 0) 
		)
		polygon 
		(
			gab "STD_BTN_POKE_HIGHLIGHT" 
			origin(0 0) 
			point(0 0) 
			point(0 22) 
			point(2 -2) 
			point(0 -18) 
			point(56 0) 
			point(2 -2) 
			point(-60 0) 
		)
	);
	
	symbol "STD_ICON_MENU_PULLDOWN"
	(
		polygon 
		(
			gab "STD_FILL_PULLDOWN_MENU" 
			origin(0 0) 
			point(0 9) 
			point(0 -9) 
			point(9 9) 
			point(-9 0) 
		)
	);
	