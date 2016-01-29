::
:: This script runs the SDP compiler on the SDP database definition contained
:: in btapp_spp_sdpdb.xml. The result is written to btapp_spp_sdpdb.h.
::


call ..\..\..\..\tools\sdp_compiler\sdp_compiler.bat -def btapp_speed-test_sdpdb.xml -out btapp_speed-test_sdpdb.h -vn sdp_db_speed_test


