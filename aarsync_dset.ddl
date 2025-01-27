
	version("WebFG Development, Builder 6.1.0 SP1");
	
	picgroup "AARSYNC_DASHBOARD_DATA"
	(
		bottom(0) 
		right(0) 
		primary_picture "AARSYNC_CB_DASHBOARD_DATA" 
		set("AARSYNC") 
		not insert 
		not delete 
		not delete_conf 
		not copy 
		picture "AARSYNC_LN_DASHBOARD_DATA" 
		(
			set("AARSYNC") 
			cam "AARSYNC_LN_AARELE_TRUE_VISIBLE" 
			set("AARSYNC") 
			
			origin(0 0) 
		)
		picture "AARSYNC_LINE_DASHBOARD_DATA" 
		(
			set("AARSYNC") 
			cam "AARSYNC_LINE_AARELE_TRUE_VISIBLE" 
			set("AARSYNC") 
			
			origin(0 0) 
		)
		picture "AARSYNC_XF_DASHBOARD_DATA" 
		(
			set("AARSYNC") 
			cam "AARSYNC_XF_AARELE_TRUE_VISIBLE" 
			set("AARSYNC") 
			
			origin(0 0) 
		)
		picture "AARSYNC_CB_DASHBOARD_DATA" 
		(
			set("AARSYNC") 
			cam "AARSYNC_CB_AARELE_TRUE_VISIBLE" 
			set("AARSYNC") 
			
			origin(0 0) 
		)
		picture "AARSYNC_ZB_DASHBOARD_DATA" 
		(
			set("AARSYNC") 
			cam "AARSYNC_ZB_AARELE_TRUE_VISIBLE" 
			set("AARSYNC") 
			
			origin(0 0) 
		)
		picture "AARSYNC_MISC_DASHBOARD_DATA" 
		(
			set("AARSYNC") 
			origin(1041 1) 
		)
		cam "AARSYNC_AARELE_VISIBLE" 
		set("AARSYNC") 
		
	);
	
	picgroup "AARSYNC_DASHBOARD_HEADER"
	(
		bottom(0) 
		right(0) 
		primary_picture "AARSYNC_DASHBOARD_HEADER" 
		set("AARSYNC") 
		not insert 
		not delete 
		not delete_conf 
		not copy 
		picture "AARSYNC_DASHBOARD_HEADER" 
		(
			set("AARSYNC") 
			origin(5 50) 
		)
		text 
		(
			gab "AARSYNC_TEXT_TITLE" 
			set("AARSYNC") 
			origin(10 15) 
			localize "AARSYNC Dashboard"
		)
		symbol "AARSYNC_ROW_SELECTED" 
		(
			set("AARSYNC") 
			cam "AARSYNC_LN_AARELE_SORT_TRUE" 
			set("AARSYNC") 
			
			origin(283 31) 
		)
		text 
		(
			gab "AARSYNC_TEXT_BOX_HEADER" 
			set("AARSYNC") 
			keyset "AARSYNC_SET_KEY230_LN_AARELE" 
			set("AARSYNC") 
			origin(296 28) 
			localize "Lines"
		)
		symbol "AARSYNC_ROW_SELECTED" 
		(
			set("AARSYNC") 
			cam "AARSYNC_XF_AARELE_SORT_TRUE" 
			set("AARSYNC") 
			
			origin(366 31) 
		)
		text 
		(
			gab "AARSYNC_TEXT_BOX_HEADER" 
			set("AARSYNC") 
			keyset "AARSYNC_SET_KEY230_XF_AARELE" 
			set("AARSYNC") 
			origin(379 28) 
			localize "Transformers"
		)
		symbol "AARSYNC_ROW_SELECTED" 
		(
			set("AARSYNC") 
			cam "AARSYNC_CB_AARELE_SORT_TRUE" 
			set("AARSYNC") 
			
			origin(487 31) 
		)
		text 
		(
			gab "AARSYNC_TEXT_BOX_HEADER" 
			set("AARSYNC") 
			keyset "AARSYNC_SET_KEY230_CB_AARELE" 
			set("AARSYNC") 
			origin(500 28) 
			localize "Switches"
		)
		symbol "AARSYNC_ROW_SELECTED" 
		(
			set("AARSYNC") 
			cam "AARSYNC_ALL_AARELE_SORT_TRUE" 
			set("AARSYNC") 
			
			origin(587 31) 
		)
		text 
		(
			gab "AARSYNC_TEXT_BOX_HEADER" 
			set("AARSYNC") 
			keyset "AARSYNC_SET_KEY230_ALL_AARELE" 
			set("AARSYNC") 
			origin(600 28) 
			localize "All"
		)
		symbol "STD_BTN_POKE_60PIX" 
		(
			set("AARSYNC") 
			keyset "AARSYNC_IMPORT_DNV_FILE" 
			set("AARSYNC") 
			origin(716 13) 
			tool_tip(localize "Force DNV Ratigns File Import")
		)
		text 
		(
			gab "STD_TEXT_TITLE_COLUMN_HEADERS" 
			set("AARSYNC") 
			origin(727 17) 
			localize "Import"
		)
		symbol "STD_BTN_POKE_60PIX" 
		(
			set("AARSYNC") 
			keyset "AARSYNC_EXPORT_DB" 
			set("AARSYNC") 
			origin(788 13) 
			tool_tip(localize "Export AAR Database into a file")
		)
		text 
		(
			gab "STD_TEXT_TITLE_COLUMN_HEADERS" 
			set("AARSYNC") 
			origin(799 17) 
			localize "Export"
		)
		symbol "STD_BTN_POKE_60PIX" 
		(
			set("AARSYNC") 
			keyset "AARSYNC_RESET_MANUAL" 
			set("AARSYNC") 
			origin(859 13) 
			tool_tip(localize "Reset all manual entries")
		)
		text 
		(
			gab "STD_TEXT_TITLE_COLUMN_HEADERS" 
			set("AARSYNC") 
			origin(872 17) 
			localize "Reset"
		)
		text 
		(
			
			cam "AARSYNC_FILEOLD_TRUE_VISIBLE" 
			set("AARSYNC") 
			gab "AARSYNC_ALARM_TEXT" 
			set("AARSYNC") 
			origin(281 6) 
			tool_tip(localize "DNV File older than %%FLOLDTH_ITEMS%% seconds")
			localize "DNV File Old"
		)
		picture "AARSYNC_APP_STATUS" 
		(
			set("AARSYNC") 
			origin(437 6) 
			tool_tip(localize "Click me to open \\"AARSYNC Log\\" popup")
		)
		symbol "AARSYNC_ICON_LEGEND" 
		(
			set("AARSYNC") 
			keyset "AARSYNC_OPEN_PARAMETERS_POPUP" 
			set("AARSYNC") 
			origin(214 22) 
			tool_tip(localize "Click me to open \\"AARSYNC Parameters\\" Popup")
		)
	);
	
	picgroup "AARSYNC_GENERAL_PARAMETERS"
	(
		bottom(0) 
		right(0) 
		primary_picture "AARSYNC_GENERAL_PARAMETERS" 
		set("AARSYNC") 
		not insert 
		not delete 
		not delete_conf 
		not copy 
		width(600)
		height(500)
		picture "AARSYNC_GENERAL_PARAMETERS" 
		(
			set("AARSYNC") 
			origin(0 0) 
		)
	);
	
	picgroup "AARSYNC_LOG_DATA"
	(
		bottom(0) 
		right(0) 
		primary_picture "AARSYNC_LOG_DATA" 
		set("AARSYNC") 
		not insert 
		not delete 
		not delete_conf 
		not copy 
		picture "AARSYNC_LOG_DATA" 
		(
			set("AARSYNC") 
			origin(0 0) 
			xlocked 
		)
	);
	
	picgroup "AARSYNC_LOG_HEADER"
	(
		bottom(0) 
		right(0) 
		primary_picture "AARSYNC_LOG_HEADER" 
		set("AARSYNC") 
		not insert 
		not delete 
		not delete_conf 
		not copy 
		picture "AARSYNC_LOG_HEADER" 
		(
			set("AARSYNC") 
			origin(5 50) 
		)
		text 
		(
			gab "AARSYNC_TEXT_TITLE" 
			set("AARSYNC") 
			origin(8 14) 
			localize "AARSYNC Log"
		)
		symbol "AARSYNC_ICON_FILTER" 
		(
			set("AARSYNC") 
			keyset "MENU" 
			set("AARSYNC") 
			menu "AARSYNC_MESSAGE_LOG_FILTER" 
			set("AARSYNC") 
			origin(321 15) 
		)
		symbol "AARSYNC_ICON_FILTER_CANCEL" 
		(
			set("AARSYNC") 
			keyset "SELECT_REMOVE_FILTER" 
			set("AARSYNC") 
			origin(351 15) 
			tool_tip(localize "Remove Filter")
		)
	);
	
	display "AARSYNC_DASHBOARD"
	(
		
		title(localize "%DIS%,%DISAPP%[%DISFAM%]     %HOST% (%VP%)     %REF%     Page: %PGT%") 
		
		application "AARSYNC"
		(
			color("0,0,0") 
		)
		color("0,0,0") 
		scale_to_fit_style(X)
		pre_command("SORT/SERVER/LAYER=DEFAULT DNV_AARELE:AARSYNC_DASHBOARD_DATA  DNV_AARELE:AARSYNC_DASHBOARD_DATA/DESCENDING") 
		horizontal_unit(10) 
		vertical_unit(10) 
		horizontal_page(50) 
		vertical_page(50) 
		refresh(2) 
		not locked_in_viewport 
		not horizontal_scroll_bar 
		not vertical_scroll_bar 
		std_menu_bar 
		not command_window 
		not on_top 
		not ret_last_tab_pnum 
		default_zoom(2.0000000) 
		tabular_layer "DEFAULT"
		(
			driving_database("AARSDB") 
			not refresh_seq 
			not refresh_occ 
			not refresh_link 
			not shade_alternate_rows 
			not refresh_second 
			not circular 
			not descending 
			not constant 
			records_per_page(0) 
			drv_min_bound (none) 
			drv_max_bound (none) 
			sort_text("") 
			constant_picgroup 
			(
				origin(0 0) 
				picgroup "AARSYNC_DASHBOARD_HEADER" 
				picgroups_per_block(1) 
				top_left_edge(bottom) 
				top_left_x(0) 
				top_left_y(0) 
				between_block_edge(bottom) 
				between_block_x(0) 
				between_block_y(0) 
				within_block_edge(bottom) 
				within_block_x(0) 
				within_block_y(0) 
				not page_head 
				not page_end 
				not page_xlocked 
				not page_ylocked 
				not free 
				not header 
				not dynamic_width 
			)
			picgroup_layout 
			(
				origin(0 0) 
				picgroup "AARSYNC_DASHBOARD_DATA" 
				picgroups_per_block(1) 
				top_left_edge(bottom) 
				top_left_x(0) 
				top_left_y(0) 
				between_block_edge(bottom) 
				between_block_x(0) 
				between_block_y(0) 
				within_block_edge(bottom) 
				within_block_x(0) 
				within_block_y(0) 
				not page_head 
				not page_end 
				not page_xlocked 
				not page_ylocked 
				not free 
				not header 
				not dynamic_width 
			)
		)
	);
	
	display "AARSYNC_LOG"
	(
		
		application "AARSYNC"
		(
			color("0,0,0") 
		)
		color("0,0,0") 
		scale_to_fit_style(X)
		horizontal_unit(10) 
		vertical_unit(10) 
		horizontal_page(50) 
		vertical_page(50) 
		refresh(2) 
		not locked_in_viewport 
		horizontal_scroll_bar 
		vertical_scroll_bar 
		std_menu_bar 
		not command_window 
		on_top 
		not ret_last_tab_pnum 
		default_zoom(2.0000000) 
		tabular_layer "DEFAULT"
		(
			driving_database("AARSLG") 
			not refresh_seq 
			not refresh_occ 
			not refresh_link 
			not shade_alternate_rows 
			not refresh_second 
			circular 
			descending 
			constant 
			records_per_page(50) 
			drv_min_bound (none) 
			drv_max_bound (none) 
			sort_text("") 
			constant_picgroup 
			(
				origin(0 0) 
				picgroup "AARSYNC_LOG_HEADER" 
				picgroups_per_block(1) 
				top_left_edge(bottom) 
				top_left_x(0) 
				top_left_y(0) 
				between_block_edge(bottom) 
				between_block_x(0) 
				between_block_y(0) 
				within_block_edge(bottom) 
				within_block_x(0) 
				within_block_y(0) 
				not page_head 
				not page_end 
				not page_xlocked 
				not page_ylocked 
				not free 
				not header 
				not dynamic_width 
			)
			picgroup_layout 
			(
				origin(0 0) 
				picgroup "AARSYNC_LOG_DATA" 
				picgroups_per_block(1) 
				top_left_edge(bottom) 
				top_left_x(5) 
				top_left_y(0) 
				between_block_edge(bottom) 
				between_block_x(0) 
				between_block_y(0) 
				within_block_edge(bottom) 
				within_block_x(0) 
				within_block_y(0) 
				not page_head 
				not page_end 
				not page_xlocked 
				not page_ylocked 
				not free 
				not header 
				not dynamic_width 
			)
		)
	);
	
	display "AARSYNC_PARAMETER"
	(
		
		title(localize "%DIS%,%DISAPP%[%DISFAM%]     %HOST% (%VP%)     %REF%     Page: %PGT%") 
		
		application "AARSYNC"
		(
			color("0,0,0") 
		)
		color("0,0,0") 
		scale_to_fit_style(XY_Or_X)
		horizontal_unit(10) 
		vertical_unit(10) 
		horizontal_page(50) 
		vertical_page(50) 
		refresh(2) 
		not locked_in_viewport 
		not horizontal_scroll_bar 
		not vertical_scroll_bar 
		not std_menu_bar 
		not command_window 
		on_top 
		not ret_last_tab_pnum 
		default_zoom(2.0000000) 
		simple_layer "DEFAULT"
		(
			picture "AARSYNC_GENERAL_PARAMETERS" 
			(
				set("AARSYNC") 
				origin(3 0) 
				xlocked 
				ylocked 
			)
		)
	);
	