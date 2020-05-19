#ifndef READ_SEGY_H
#define READ_SEGY_H

#include "gen.hxx"
/////////////////////////////////////////////////////////////////////////////
// SEGY Defines
/////////////////////////////////////////////////////////////////////////////

#define SEGY_EBCDIC_HDR_SIZE 3200
#define SEGY_REEL_HDR_SIZE 400
#define SEGY_TRACE_HDR_SIZE 240

#ifdef MSDOS

#ifndef UINT8_T
#define UINT8_T
typedef unsigned char 	uint8_t;
#endif

#ifndef INT16_T
#define INT16_T 
typedef short	int16_t;
#endif

#ifndef INT32_T
#define INT32_T 
typedef long int32_t;
#endif

#endif 


typedef struct segy_ebcdic_hdr {
	uint8_t buffer[SEGY_EBCDIC_HDR_SIZE];
} segy_ebcdic_hdr;

typedef struct segy_reel_hdr {
	// segy reel id - 12 bytes
	int32_t job_id_number;
	int32_t line_number;
	int32_t reel_number;
	// segy reel layout - 12 bytes
	int16_t traces_per_record;
	int16_t aux_traces_per_record;
	int16_t sample_data_interval_ms;
	int16_t original_data_interval_ms;
	int16_t samples_per_trace;
	int16_t original_samples_per_trace;
	// segy description - 36 bytes
	int16_t data_sample_format_code;
	int16_t CDP_fold;
	int16_t trace_sorting_code;
	int16_t vertical_sum_code;
	int16_t sweep_frequency_start_hz;
	int16_t sweep_frequency_end_hz;
	int16_t sweep_length_ms;
	int16_t sweep_type_code;
	int16_t trace_number_of_sweep_channel;
	int16_t sweep_trace_taper_length_start_ms;
	int16_t sweep_trace_taper_length_end_ms;
	int16_t taper_type_code;
	int16_t correlated_data_traces_flag;
	int16_t binary_gain_recovered_flag;
	int16_t amplitude_recovery_method_code;
	int16_t measurement_system;
	int16_t impulse_signal_polarity;
	int16_t vibratory_polarity_code;
	uint8_t buffer[340]; // 12 bytes + 12 bytes + 36 bytes + 340 buffer bytes = REEL_HDR_SIZE
} segy_reel_hdr;



/*
The SEG-Y Binary Reel Header

The binary reel header contains much information about the data; Much of
this information is optional, that is, the entire header is not required
to be valid; In fact, none of it is required to be valid, although some
fields are strongly recommended;

The 400 bytes contain 2-byte and 4-byte integers in the following layout:

Bytes         Description

001 - 004     Job identification number;
005 - 008  *  Line number;
009 - 012  *  Reel number;
013 - 014  *  Number of data traces per record;
015 - 016  *  Number of auxiliary traces per record;
017 - 018  *  Sample interval of this reel's data in microseconds;
019 - 020     Sample interval of original field recording in microseconds;
021 - 022  *  Number of samples per trace for this reel's data;
023 - 024     Number of samples per trace in original field recording;
025 - 026  *  Data sample format code:
		  1 = 32-bit IBM floating point
		  2 = 32-bit fixed-point (integer)
		  3 = 16-bit fixed-point (integer)
		  4 = 32-bit fixed-point with gain code (integer)
027 - 028  *  CDP fold (expected number of data traces per ensemble);
029 - 030     Trace sorting code:
		  1 = as recorded
		  2 = CDP ensemble
		  3 = single fold continuous profile
		  4 = horizontally stacked
031 - 032     Vertical sum code (1 = no sum, 2 = two sum, ;;;)
033 - 034     Sweep frequency at start in Hertz;
035 - 036     Sweep frequency at end in Hertz;
037 - 038     Sweep length in milliseconds;
039 - 040     Sweep type code:
		  1 = linear
		  2 = parabolic
		  3 = exponential
		  4 = other
041 - 042     Trace number of sweep channel;
043 - 044     Sweep trace taper length at start in milliseconds;
045 - 046     Sweep trace taper length at end in milliseconds;
047 - 048     Taper type code:
		  1 = linear
		  2 = cosine squared
		  3 = other
049 - 050     Correlated data traces (1 = no, 2 = yes);
051 - 052     Binary gain recovered (1 = yes, 2 = no);
053 - 054     Amplitude recovery method code:
		  1 = one
		  2 = spherical divergence
		  3 = AGC
		  4 = other
055 - 056  *  Measurement system (1 = meters, 2 = feet);
057 - 058     Impulse signal polarity (increase in pressure or upward
	      geophone case movement gives 1=negative or 2=positive number);
059 - 060     Vibratory polarity code (seismic lags pilot signal by):
		  1 = 337;5 to 22;5 degrees
		  2 = 22;5 to 67;5 degrees
		  3 = 67;5 to 112;5 degrees
		  4 = 112;5 to 157;5 degrees
		  5 = 157;5 to 202;5 degrees
		  6 = 202;5 to 247;5 degrees
		  7 = 247;5 to 292;5 degrees
		  8 = 292;5 to 337;5 degrees
061 - 400     Unassigned (for optional information);
*/

typedef struct segy_trace_hdr {
	// trace info
	// 7 * 4 = 28 bytes
	int32_t trace_sequence_number_within_line;
	int32_t trace_sequence_number_within_reel;
	int32_t original_field_record_number;
	int32_t trace_sequence_number_within_original_field_record;
	int32_t energy_source_point_number;
	int32_t cdp_ensemble_number;
	int32_t trace_sequence_number_within_cdp_ensemble;
	// 4 * 2 = 8 bytes
	int16_t trace_identification_code;
	int16_t number_of_vertically_summed_traces_yielding_this_trace;
	int16_t number_of_horizontally_stacked_traced_yielding_this_trace;
	int16_t data_use;
	// location, geometry info 
	// 8 * 4 = 32 bytes
	int32_t distance_from_source_point_to_receiver_group;
	int32_t receiver_group_elevation;
	int32_t surface_elevation_at_source;
	int32_t source_depth_below_surface;
	int32_t datum_elevation_at_receiver_group;
	int32_t datum_elevation_at_source;
	int32_t water_depth_at_source;
	int32_t water_depth_at_receiver_group;
	// 2 * 2 = 4 bytes
	int16_t scalar_for_elevations_and_depths;
	int16_t scalar_for_coordinates;
	// 4 * 4 = 16 bytes
	int32_t x_source_coordinate;
	int32_t y_source_coordinate;
	int32_t x_receiver_group_coordinate;
	int32_t y_receiver_group_coordinate;
	// 2 bytes
	int16_t coordinate_units;
	 
	int16_t weathering_velocity;
	int16_t subweathering_velocity;
	int16_t uphole_time_at_source;
	int16_t uphole_time_at_group;
	int16_t source_static_correction;

	int16_t group_static_correction;
	int16_t total_static_applied;
	int16_t lag_time_a;
	int16_t lag_time_b;
	int16_t delay_according_time;

	int16_t brute_time_start;
	int16_t mute_time_end;
	int16_t samples_in_this_trace;
	int16_t sample_intervall;
	int16_t gain_type_instruments;

	uint8_t buffer[120];
} segy_trace_hdr;

/*
The SEG-Y Trace Header

The 240-byte binary trace header consists of 2-byte and 4-byte integers
in the following layout:

Bytes         Description

001 - 004  *  Trace sequence number within line;
005 - 008     Trace sequence number within reel;
009 - 012  *  Original field record number;
013 - 016  *  Trace sequence number within original field record;
017 - 020     Energy source point number;
021 - 024     CDP ensemble number;
025 - 028     Trace sequence number within CDP ensemble;
029 - 030  *  Trace identification code:
		  1 = seismic data
		  2 = dead
		  3 = dummy
		  4 = time break
		  5 = uphole
		  6 = sweep
		  7 = timing
		  8 = water break
		  9+ = optional use
031 - 032     Number of vertically summed traces yielding this trace;
033 - 034     Number of horizontally stacked traced yielding this trace;
035 - 036     Data use (1 = production, 2 = test);
037 - 040     Distance from source point to receiver group;
041 - 044     Receiver group elevation;
045 - 048     Surface elevation at source;
049 - 052     Source depth below surface;
053 - 056     Datum elevation at receiver group;
057 - 060     Datum elevation at source;
061 - 064     Water depth at source;
065 - 068     Water depth at receiver group;
069 - 070     Scalar for elevations and depths (+ = multiplier, - = divisor);
071 - 072     Scalar for coordinates (+ = multiplier, - = divisor);
073 - 076     X source coordinate;
077 - 080     Y source coordinate;
081 - 084     X receiver group coordinate;
085 - 088     Y receiver group coordinate;
089 - 090     Coordinate units (1 = length in meters or feet, 2 = arc seconds);
091 - 092     Weathering velocity;
093 - 094     Subweathering velocity;
095 - 096     Uphole time at source;
097 - 098     Uphole time at receiver group;
099 - 100     Source static correction;
101 - 102     Receiver group static correction;
103 - 104     Total static applied;
105 - 106     Lag time between end of header and time break in milliseconds;
107 - 108     Lag time between time break and shot in milliseconds;
109 - 110     Lag time beteen shot and recording start in milliseconds;
111 - 112     Start of mute time;
113 - 114     End of mute time;
115 - 116  *  Number of samples in this trace;
117 - 118  *  Sample interval of this trace in microseconds;
119 - 120     Field instrument gain type code:
		  1 = fixed
		  2 = binary
		  3 = floating point
		  4+ = optional use
121 - 122     Instrument gain constant;
123 - 124     Intrument early gain in decibels;
125 - 126     Correlated (1 = no, 2 = yes);
127 - 128     Sweep frequency at start;
129 - 130     Sweep fequency at end;
131 - 132     Sweep length in milliseconds;
133 - 134     Sweep type code:
		  1 = linear
		  2 = parabolic
		  3 = exponential
		  4 = other
135 - 136     Sweep taper trace length at start in milliseconds;
137 - 138     Sweep taper trace length at end in milliseconds;
139 - 140     Taper type code:
		  1 = linear
		  2 = cosine squared
		  3 = other
141 - 142     Alias filter frequency;
143 - 144     Alias filter slope;
145 - 146     Notch filter frequency;
147 - 148     Notch filter slope;
149 - 150     Low cut frequency;
151 - 152     High cut frequency;
153 - 154     Low cut slope;
155 - 156     High cut slope;
157 - 158     Year data recorded;
159 - 160     Day of year;
161 - 162     Hour of day (24-hour clock);
163 - 164     Minute of hour;
165 - 166     Second of minute;
167 - 168     Time basis (1 = local, 2 = GMT, 3 = other);
169 - 170     Trace weighting factor for fixed-point format data;
171 - 172     Geophone group number of roll switch position one;
173 - 174     Geophone group number of first trace of original field record;
175 - 176     Geophone group number of last trace of original field record;
177 - 178     Gap size (total number of groups dropped);
179 - 180     Overtravel associated with taper (1 = down/behind, 2 = up/ahead);
181 - 240     Unassigned (for optional information);
*/

void DUMP_reel_hdr (segy_reel_hdr *hdr);
void DUMP_trace_hdr (segy_trace_hdr *hdr);

#endif
