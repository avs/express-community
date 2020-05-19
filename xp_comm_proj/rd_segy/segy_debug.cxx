#include "rd_segy.h"

void DUMP_reel_hdr (segy_reel_hdr *hdr){
	// segy reel id - 12 bytes
	fprintf(stderr,"Reel Header \n\n");

	fprintf(stderr,"job_id_number: %i\n",hdr->job_id_number);
	fprintf(stderr,"line_number: %i\n",hdr->line_number);
	fprintf(stderr,"reel_number: %i\n",hdr->reel_number);

	// segy reel layout - 12 bytes
	fprintf(stderr,"traces_per_record: %i\n",hdr->traces_per_record);
	fprintf(stderr,"aux_traces_per_record: %i\n",hdr->aux_traces_per_record);
	fprintf(stderr,"sample_data_interval_ms: %i\n",hdr->sample_data_interval_ms);
	fprintf(stderr,"original_data_interval_ms: %i\n",hdr->original_data_interval_ms);
	fprintf(stderr,"samples_per_trace: %i\n",hdr->samples_per_trace);
	fprintf(stderr,"original_samples_per_trace: %i\n",hdr->original_samples_per_trace);

	// segy description - 36 bytes
	fprintf(stderr,"data_sample_format_code: %i\n",hdr->data_sample_format_code);
	fprintf(stderr,"CDP_fold: %i\n",hdr->CDP_fold);
	fprintf(stderr,"trace_sorting_code: %i\n",hdr->trace_sorting_code);
	fprintf(stderr,"vertical_sum_code: %i\n",hdr->vertical_sum_code);
	fprintf(stderr,"sweep_frequency_start_hz: %i\n",hdr->sweep_frequency_start_hz);
	fprintf(stderr,"sweep_frequency_end_hz: %i\n",hdr->sweep_frequency_end_hz);
	fprintf(stderr,"sweep_length_ms: %i\n",hdr->sweep_length_ms);
	fprintf(stderr,"sweep_type_code: %i\n",hdr->sweep_type_code);
	fprintf(stderr,"trace_number_of_sweep_channel: %i\n",hdr->trace_number_of_sweep_channel);
	fprintf(stderr,"sweep_trace_taper_length_start_ms: %i\n",hdr->sweep_trace_taper_length_start_ms);
	fprintf(stderr,"sweep_trace_taper_length_end_ms: %i\n",hdr->sweep_trace_taper_length_end_ms);
	fprintf(stderr,"taper_type_code: %i\n",hdr->taper_type_code);
	fprintf(stderr,"correlated_data_traces_flag: %i\n",hdr->correlated_data_traces_flag);
	fprintf(stderr,"binary_gain_recovered_flag: %i\n",hdr->binary_gain_recovered_flag);
	fprintf(stderr,"amplitude_recovery_method_code: %i\n",hdr->amplitude_recovery_method_code);
	fprintf(stderr,"measurement_system: %i\n",hdr->measurement_system);
	fprintf(stderr,"impulse_signal_polarity: %i\n",hdr->impulse_signal_polarity);
	fprintf(stderr,"vibratory_polarity_code: %i\n",hdr->vibratory_polarity_code);
	fprintf(stderr,"buffer: %s\n",hdr->buffer);
	fflush(stderr);
}

void DUMP_trace_hdr (segy_trace_hdr *hdr){
	// trace info
	fprintf(stderr,"Trace Header\n\n");

	fprintf(stderr,"trace_sequence_number_within_line: %i\n",hdr->trace_sequence_number_within_line);
	fprintf(stderr,"trace_sequence_number_within_reel: %i\n",hdr->trace_sequence_number_within_reel);
	fprintf(stderr,"original_field_record_number: %i\n",hdr->original_field_record_number);
	fprintf(stderr,"trace_sequence_number_within_original_field_record: %i\n",hdr->trace_sequence_number_within_original_field_record);
	fprintf(stderr,"energy_source_point_number: %i\n",hdr->energy_source_point_number);
	fprintf(stderr,"cdp_ensemble_number: %i\n",hdr->cdp_ensemble_number);
	fprintf(stderr,"trace_sequence_number_within_cdp_ensemble: %i\n",hdr->trace_sequence_number_within_cdp_ensemble);
	fprintf(stderr,"trace_identification_code: %i\n",hdr->trace_identification_code);
	fprintf(stderr,"number_of_vertically_summed_traces_yielding_this_trace: %i\n",hdr->number_of_vertically_summed_traces_yielding_this_trace);
	fprintf(stderr,"number_of_horizontally_stacked_traced_yielding_this_trace: %i\n",hdr->number_of_horizontally_stacked_traced_yielding_this_trace);
	fprintf(stderr,"data_use: %i\n",hdr->data_use);

	// location, geometry info 
	fprintf(stderr,"distance_from_source_point_to_receiver_group: %i\n",hdr->distance_from_source_point_to_receiver_group);
	fprintf(stderr,"receiver_group_elevation: %i\n",hdr->receiver_group_elevation);
	fprintf(stderr,"surface_elevation_at_source: %i\n",hdr->surface_elevation_at_source);
	fprintf(stderr,"source_depth_below_surface: %i\n",hdr->source_depth_below_surface);
	fprintf(stderr,"datum_elevation_at_receiver_group: %i\n",hdr->datum_elevation_at_receiver_group);
	fprintf(stderr,"datum_elevation_at_source: %i\n",hdr->datum_elevation_at_source);
	fprintf(stderr,"water_depth_at_source: %i\n",hdr->water_depth_at_source);
	fprintf(stderr,"water_depth_at_receiver_group: %i\n",hdr->water_depth_at_receiver_group);
	fprintf(stderr,"scalar_for_elevations_and_depths: %i\n",hdr->scalar_for_elevations_and_depths);
	fprintf(stderr,"scalar_for_coordinates: %i\n",hdr->scalar_for_coordinates);
	fprintf(stderr,"x_source_coordinate: %i\n",hdr->x_source_coordinate);
	fprintf(stderr,"y_source_coordinate: %i\n",hdr->y_source_coordinate);
	fprintf(stderr,"x_receiver_group_coordinate: %i\n",hdr->x_receiver_group_coordinate);
	fprintf(stderr,"y_receiver_group_coordinate: %i\n",hdr->y_receiver_group_coordinate);
	fprintf(stderr,"coordinate_units: %i\n",hdr->coordinate_units);
	
	fprintf(stderr,"weathering_velocity: %i\n",hdr->weathering_velocity);
	fprintf(stderr,"subweathering_velocity: %i\n",hdr->subweathering_velocity);
	fprintf(stderr,"uphole_time_at_source: %i\n",hdr->uphole_time_at_source);
	fprintf(stderr,"uphole_time_at_group: %i\n",hdr->uphole_time_at_group);
	fprintf(stderr,"source_static_correction: %i\n",hdr->source_static_correction);

	fprintf(stderr,"group_static_correction: %i\n",hdr->group_static_correction);
	fprintf(stderr,"total_static_applied: %i\n",hdr->total_static_applied);
	fprintf(stderr,"lag_time_a: %i\n",hdr->lag_time_a);
	fprintf(stderr,"lag_time_b: %i\n",hdr->lag_time_b);
	fprintf(stderr,"delay_according_time: %i\n",hdr->delay_according_time);

	fprintf(stderr,"brute_time_start: %i\n",hdr->brute_time_start);
	fprintf(stderr,"mute_time_end: %i\n",hdr->mute_time_end);
	fprintf(stderr,"samples_in_this_trace: %i\n",hdr->samples_in_this_trace);
	fprintf(stderr,"sample_intervall: %i\n",hdr->sample_intervall);
	fprintf(stderr,"gain_type_instruments: %i\n",hdr->gain_type_instruments);
	
	fprintf(stderr,"buffer: %s\n",hdr->buffer);
	fflush(stderr);
}
