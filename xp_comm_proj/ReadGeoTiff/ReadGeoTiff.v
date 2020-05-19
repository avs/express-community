
flibrary ReadGeoTiff <build_dir="xp_comm_proj/ReadGeoTiff",
                    libdeps="GSGTIFFLIB">
{
   // Libraries
   "../xp_comm_proj/ReadGeoTiff/libtiff/TIFF34LIB.v" TIFF34LIB;

   "../xp_comm_proj/ReadGeoTiff/ReadGeoTiff_mods.v" ReadGeoTiffMods;
   "../xp_comm_proj/ReadGeoTiff/ReadGeoTiff_macs.v" ReadGeoTiffMacs;
   "../xp_comm_proj/ReadGeoTiff/ReadGeoTiff_apps.v" ReadGeoTiffApps;

}; // End of flibrary
