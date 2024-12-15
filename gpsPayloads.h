#ifndef GPSPAYLOADS_H
#define GPSPAYLOADS_H

const unsigned char UBX_HEADER[] = { 0xB5, 0x62 };
const unsigned char NAV_DOP_HEADER[] = { 0x01, 0x04 };
const unsigned char NAV_PVT_HEADER[] = { 0x01, 0x07 };
const unsigned char NAV_ODO_HEADER[] = { 0x01, 0x09 };
const unsigned char NAV_SAT_HEADER[] = { 0x01, 0x35 };

enum _ubxMsgType {
  MT_NONE,
  MT_NAV_DOP,
  MT_NAV_PVT
};

struct NAV_DOP {
  unsigned char cls;           // Payload class
  unsigned char id;            // Payload ID
  unsigned short len;          // Payload length
  unsigned long iTOW;          // GPS time of week of the navigation epoch (ms)

  unsigned short gDOP;         // Geometric DOP
  unsigned short pDOP;         // Position DOP
  unsigned short tDOP;         // Time DOP
  unsigned short vDOP;         // Vertical DOP
  unsigned short hDOP;         // Horizontal DOP
  unsigned short nDOP;         // Northing DOP
  unsigned short eDOP;         // Easting DOP
};

struct NAV_PVT {
  unsigned char cls;           // Payload class
  unsigned char id;            // Payload ID
  unsigned short len;          // Payload length
  unsigned long iTOW;          // GPS time of week of the navigation epoch (ms)
  
  unsigned short year;         // Year (UTC) 
  unsigned char month;         // Month, range 1..12 (UTC)
  unsigned char day;           // Day of month, range 1..31 (UTC)
  unsigned char hour;          // Hour of day, range 0..23 (UTC)
  unsigned char minute;        // Minute of hour, range 0..59 (UTC)
  unsigned char second;        // Seconds of minute, range 0..60 (UTC)
  char valid;                  // Validity Flags (see graphic below)
  unsigned long tAcc;          // Time accuracy estimate (UTC) (ns)
  long nano;                   // Fraction of second, range -1e9 .. 1e9 (UTC) (ns)
  unsigned char fixType;       // GNSSfix Type, range 0..5
  unsigned char flags;         // Fix Status Flags
  unsigned char flags2;        // Additional flags 
  unsigned char numSV;         // Number of satellites used in Nav Solution
  
  long lon;                    // Longitude (deg)
  long lat;                    // Latitude (deg)
  long height;                 // Height above Ellipsoid (mm)
  long hMSL;                   // Height above mean sea level (mm)
  unsigned long hAcc;          // Horizontal Accuracy Estimate (mm)
  unsigned long vAcc;          // Vertical Accuracy Estimate (mm)
  
  long velN;                   // NED north velocity (mm/s)
  long velE;                   // NED east velocity (mm/s)
  long velD;                   // NED down velocity (mm/s)
  long gSpeed;                 // Ground Speed (2-D) (mm/s)
  long heading;                // Heading of motion 2-D (deg)
  unsigned long sAcc;          // Speed Accuracy Estimate
  unsigned long headingAcc;    // Heading Accuracy Estimate
  unsigned short pDOP;         // Position dilution of precision
  
  unsigned char flags3;        // Additional flags
  unsigned char reserved1[5];  // Reserved
  long headVeh;                // Heading of vehicle (2-D)
  short magDec;                // Magnetic declination
  unsigned short magAcc;       // Magnetic declination accuracy
};


union UBXMessage {
  NAV_DOP navDOP;
  NAV_PVT navPVT;
};

#endif // GPSPAYLOADS_H
