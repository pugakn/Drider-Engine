#pragma once
#include "dr_sound_prerequisites.h"

namespace driderSDK {

  namespace DR_SOUND_MODE {
    enum E {
      kDrMode_DEFAULT = 0x00000000,
      kDrMode_LOOP_OFF = 0x00000001,
      kDrMode_LOOP_NORMAL = 0x00000002,
      kDrMode_LOOP_BIDI = 0x00000004,
      kDrMode_2D = 0x00000008,
      kDrMode_3D = 0x00000010,
      kDrMode_CREATESTREAM = 0x00000080,
      kDrMode_CREATESAMPLE = 0x00000100,
      kDrMode_CREATECOMPRESSEDSAMPLE = 0x00000200,
      kDrMode_OPENUSER = 0x00000400,
      kDrMode_OPENMEMORY = 0x00000800,
      kDrMode_OPENMEMORY_POINT = 0x10000000,
      kDrMode_OPENRAW = 0x00001000,
      kDrMode_OPENONLY = 0x00002000,
      kDrMode_ACCURATETIME = 0x00004000,
      kDrMode_MPEGSEARCH = 0x00008000,
      kDrMode_NONBLOCKING = 0x00010000,
      kDrMode_UNIQUE = 0x00020000,
      kDrMode_3D_HEADRELATIVE = 0x00040000,
      kDrMode_3D_WORLDRELATIVE = 0x00080000,
      kDrMode_3D_INVERSEROLLOFF = 0x00100000,
      kDrMode_3D_LINEARROLLOFF = 0x00200000,
      kDrMode_3D_LINEARSQUAREROLLOFF = 0x00400000,
      kDrMode_3D_INVERSETAPEREDROLLOFF = 0x00800000,
      kDrMode_3D_CUSTOMROLLOFF = 0x04000000,
      kDrMode_3D_IGNOREGEOMETRY = 0x40000000,
      kDrMode_IGNORETAGS = 0x02000000,
      kDrMode_LOWMEM = 0x08000000,
      kDrMode_LOADSECONDARYRAM = 0x20000000,
      kDrMode_VIRTUAL_PLAYFROMSTART = 0x80000000
    };    
  };

  namespace DR_INITFLAGS {
    enum E
    {
      kDrInitFlags_NORMAL = 0x00000000,
      kDrInitFlags_STREAM_FROM_UPDATE = 0x00000001,
      kDrInitFlags_MIX_FROM_UPDATE = 0x00000002,
      kDrInitFlags_3D_RIGHTHANDED = 0x00000004,
      kDrInitFlags_CHANNEL_LOWPASS = 0x00000100,
      kDrInitFlags_CHANNEL_DISTANCEFILTER = 0x00000200,
      kDrInitFlags_PROFILE_ENABLE = 0x00010000,
      kDrInitFlags_VOL0_BECOMES_VIRTUAL = 0x00020000,
      kDrInitFlags_GEOMETRY_USECLOSEST = 0x00040000,
      kDrInitFlags_PREFER_DOLBY_DOWNMIX = 0x00080000,
      kDrInitFlags_THREAD_UNSAFE = 0x00100000,
      kDrInitFlags_PROFILE_METER_ALL = 0x00200000,
      kDrInitFlags_DISABLE_SRS_HIGHPASSFILTER = 0x00400000
    };
  };

  namespace DR_TIMEUNIT {
    enum E {
      kDrTimeUnit_MS  = 0x00000001,
      kDrTimeUnit_PCM  = 0x00000002,
      kDrTimeUnit_PCMBYTES =  0x00000004,
      kDrTimeUnit_RAWBYTES =  0x00000008,
      kDrTimeUnit_PCMFRACTION = 0x00000010,
      kDrTimeUnit_MODORDER = 0x00000100,
      kDrTimeUnit_MODROW = 0x00000200,
      kDrTimeUnit_MODPATTERN = 0x00000400,
      kDrTimeUnit_BUFFERED = 0x10000000
    };
  };

  namespace DR_SOUND_FORMAT {
    enum E {
      DR_SOUND_FORMAT_NONE,
      DR_SOUND_FORMAT_PCM8,
      DR_SOUND_FORMAT_PCM16,
      DR_SOUND_FORMAT_PCM24,
      DR_SOUND_FORMAT_PCM32,
      DR_SOUND_FORMAT_PCMFLOAT,
      DR_SOUND_FORMAT_BITSTREAM,
      DR_SOUND_FORMAT_MAX
    };
  };

  namespace DR_SOUND_RESULT {
    enum E {
      kDrResult_OK,
      kDrResult_ERR_BADCOMMAND,
      kDrResult_ERR_CHANNEL_ALLOC,
      kDrResult_ERR_CHANNEL_STOLEN,
      kDrResult_ERR_DMA,
      kDrResult_ERR_DSP_CONNECTION,
      kDrResult_ERR_DSP_DONTPROCESS,
      kDrResult_ERR_DSP_FORMAT,
      kDrResult_ERR_DSP_INUSE,
      kDrResult_ERR_DSP_NOTFOUND,
      kDrResult_ERR_DSP_RESERVED,
      kDrResult_ERR_DSP_SILENCE,
      kDrResult_ERR_DSP_TYPE,
      kDrResult_ERR_FILE_BAD,
      kDrResult_ERR_FILE_COULDNOTSEEK,
      kDrResult_ERR_FILE_DISKEJECTED,
      kDrResult_ERR_FILE_EOF,
      kDrResult_ERR_FILE_ENDOFDATA,
      kDrResult_ERR_FILE_NOTFOUND,
      kDrResult_ERR_FORMAT,
      kDrResult_ERR_HEADER_MISMATCH,
      kDrResult_ERR_HTTP,
      kDrResult_ERR_HTTP_ACCESS,
      kDrResult_ERR_HTTP_PROXY_AUTH,
      kDrResult_ERR_HTTP_SERVER_ERROR,
      kDrResult_ERR_HTTP_TIMEOUT,
      kDrResult_ERR_INITIALIZATION,
      kDrResult_ERR_INITIALIZED,
      kDrResult_ERR_INTERNAL,
      kDrResult_ERR_INVALID_FLOAT,
      kDrResult_ERR_INVALID_HANDLE,
      kDrResult_ERR_INVALID_PARAM,
      kDrResult_ERR_INVALID_POSITION,
      kDrResult_ERR_INVALID_SPEAKER,
      kDrResult_ERR_INVALID_SYNCPOINT,
      kDrResult_ERR_INVALID_THREAD,
      kDrResult_ERR_INVALID_VECTOR,
      kDrResult_ERR_MAXAUDIBLE,
      kDrResult_ERR_MEMORY,
      kDrResult_ERR_MEMORY_CANTPOINT,
      kDrResult_ERR_NEEDS3D,
      kDrResult_ERR_NEEDSHARDWARE,
      kDrResult_ERR_NET_CONNECT,
      kDrResult_ERR_NET_SOCKET_ERROR,
      kDrResult_ERR_NET_URL,
      kDrResult_ERR_NET_WOULD_BLOCK,
      kDrResult_ERR_NOTREADY,
      kDrResult_ERR_OUTPUT_ALLOCATED,
      kDrResult_ERR_OUTPUT_CREATEBUFFER,
      kDrResult_ERR_OUTPUT_DRIVERCALL,
      kDrResult_ERR_OUTPUT_FORMAT,
      kDrResult_ERR_OUTPUT_INIT,
      kDrResult_ERR_OUTPUT_NODRIVERS,
      kDrResult_ERR_PLUGIN,
      kDrResult_ERR_PLUGIN_MISSING,
      kDrResult_ERR_PLUGIN_RESOURCE,
      kDrResult_ERR_PLUGIN_VERSION,
      kDrResult_ERR_RECORD,
      kDrResult_ERR_REVERB_CHANNELGROUP,
      kDrResult_ERR_REVERB_INSTANCE,
      kDrResult_ERR_SUBSOUNDS,
      kDrResult_ERR_SUBSOUND_ALLOCATED,
      kDrResult_ERR_SUBSOUND_CANTMOVE,
      kDrResult_ERR_TAGNOTFOUND,
      kDrResult_ERR_TOOMANYCHANNELS,
      kDrResult_ERR_TRUNCATED,
      kDrResult_ERR_UNIMPLEMENTED,
      kDrResult_ERR_UNINITIALIZED,
      kDrResult_ERR_UNSUPPORTED,
      kDrResult_ERR_VERSION,
      kDrResult_ERR_EVENT_ALREADY_LOADED,
      kDrResult_ERR_EVENT_LIVEUPDATE_BUSY,
      kDrResult_ERR_EVENT_LIVEUPDATE_MISMATCH,
      kDrResult_ERR_EVENT_LIVEUPDATE_TIMEOUT,
      kDrResult_ERR_EVENT_NOTFOUND,
      kDrResult_ERR_STUDIO_UNINITIALIZED,
      kDrResult_ERR_STUDIO_NOT_LOADED,
      kDrResult_ERR_INVALID_STRING,
      kDrResult_ERR_ALREADY_LOCKED,
      kDrResult_ERR_NOT_LOCKED,
      kDrResult_ERR_RECORD_DISCONNECTED,
      kDrResult_ERR_TOOMANYSAMPLES
    }; 
  };

  namespace DR_SOUND_TYPE {
    enum E {
      DR_SOUND_TYPE_UNKNOWN,         /* 3rd party / unknown plugin format. */
      DR_SOUND_TYPE_AIFF,            /* AIFF. */
      DR_SOUND_TYPE_ASF,             /* Microsoft Advanced Systems Format (ie WMA/ASF/WMV). */
      DR_SOUND_TYPE_DLS,             /* Sound font / downloadable sound bank. */
      DR_SOUND_TYPE_FLAC,            /* FLAC lossless codec. */
      DR_SOUND_TYPE_FSB,             /* FMOD Sample Bank. */
      DR_SOUND_TYPE_IT,              /* Impulse Tracker. */
      DR_SOUND_TYPE_MIDI,            /* MIDI. */
      DR_SOUND_TYPE_MOD,             /* Protracker / Fasttracker MOD. */
      DR_SOUND_TYPE_MPEG,            /* MP2/MP3 MPEG. */
      DR_SOUND_TYPE_OGGVORBIS,       /* Ogg vorbis. */
      DR_SOUND_TYPE_PLAYLIST,        /* Information only from ASX/PLS/M3U/WAX playlists */
      DR_SOUND_TYPE_RAW,             /* Raw PCM data. */
      DR_SOUND_TYPE_S3M,             /* ScreamTracker 3. */
      DR_SOUND_TYPE_USER,            /* User created sound. */
      DR_SOUND_TYPE_WAV,             /* Microsoft WAV. */
      DR_SOUND_TYPE_XM,              /* FastTracker 2 XM. */
      DR_SOUND_TYPE_XMA,             /* Xbox360 XMA */
      DR_SOUND_TYPE_AUDIOQUEUE,      /* iPhone hardware decoder, supports AAC, ALAC and MP3. */
      DR_SOUND_TYPE_AT9,             /* PS4 / PSVita ATRAC 9 format */
      DR_SOUND_TYPE_VORBIS,          /* Vorbis */
      DR_SOUND_TYPE_MEDIA_FOUNDATION,/* Windows Store Application built in system codecs */
      DR_SOUND_TYPE_MEDIACODEC,      /* Android MediaCodec */
      DR_SOUND_TYPE_FADPCM,          /* FMOD Adaptive Differential Pulse Code Modulation */
     
      DR_SOUND_TYPE_MAX,             /* Maximum number of sound types supported. */
      DR_SOUND_TYPE_FORCEINT = 65536 /* Makes sure this enum is signed 32bit. */
    };
  };

  namespace DR_CHANNELORDER {
    enum E {
      FMOD_CHANNELORDER_DEFAULT,
      FMOD_CHANNELORDER_WAVEFORMAT,
      FMOD_CHANNELORDER_PROTOOLS,
      FMOD_CHANNELORDER_ALLMONO,
      FMOD_CHANNELORDER_ALLSTEREO,
      FMOD_CHANNELORDER_ALSA,
      FMOD_CHANNELORDER_MAX
    };
  };

  namespace DR_CHANNELMASK {
    enum E {
      DR_CHANNELMASK_FRONT_LEFT = 0x00000001,
      DR_CHANNELMASK_FRONT_RIGHT = 0x00000002,
      DR_CHANNELMASK_FRONT_CENTER = 0x00000004,
      DR_CHANNELMASK_LOW_FREQUENCY = 0x00000008,
      DR_CHANNELMASK_SURROUND_LEFT = 0x00000010,
      DR_CHANNELMASK_SURROUND_RIGHT = 0x00000020,
      DR_CHANNELMASK_BACK_LEFT = 0x00000040,
      DR_CHANNELMASK_BACK_RIGHT = 0x00000080,
      DR_CHANNELMASK_BACK_CENTER = 0x00000100,
      DR_CHANNELMASK_MONO = DR_CHANNELMASK_FRONT_LEFT,
      DR_CHANNELMASK_STEREO = (DR_CHANNELMASK_FRONT_LEFT | DR_CHANNELMASK_FRONT_RIGHT),
      DR_CHANNELMASK_LRC = (DR_CHANNELMASK_FRONT_LEFT | DR_CHANNELMASK_FRONT_RIGHT | DR_CHANNELMASK_FRONT_CENTER),
      DR_CHANNELMASK_QUAD = (DR_CHANNELMASK_FRONT_LEFT | DR_CHANNELMASK_FRONT_RIGHT | DR_CHANNELMASK_SURROUND_LEFT | DR_CHANNELMASK_SURROUND_RIGHT),
      DR_CHANNELMASK_SURROUND = (DR_CHANNELMASK_FRONT_LEFT | DR_CHANNELMASK_FRONT_RIGHT | DR_CHANNELMASK_FRONT_CENTER | DR_CHANNELMASK_SURROUND_LEFT | DR_CHANNELMASK_SURROUND_RIGHT),
      DR_CHANNELMASK_5POINT1 = (DR_CHANNELMASK_FRONT_LEFT | DR_CHANNELMASK_FRONT_RIGHT | DR_CHANNELMASK_FRONT_CENTER | DR_CHANNELMASK_LOW_FREQUENCY | DR_CHANNELMASK_SURROUND_LEFT | DR_CHANNELMASK_SURROUND_RIGHT),
      DR_CHANNELMASK_5POINT1_REARS = (DR_CHANNELMASK_FRONT_LEFT | DR_CHANNELMASK_FRONT_RIGHT | DR_CHANNELMASK_FRONT_CENTER | DR_CHANNELMASK_LOW_FREQUENCY | DR_CHANNELMASK_BACK_LEFT | DR_CHANNELMASK_BACK_RIGHT),
      DR_CHANNELMASK_7POINT0 = (DR_CHANNELMASK_FRONT_LEFT | DR_CHANNELMASK_FRONT_RIGHT | DR_CHANNELMASK_FRONT_CENTER | DR_CHANNELMASK_SURROUND_LEFT | DR_CHANNELMASK_SURROUND_RIGHT | DR_CHANNELMASK_BACK_LEFT | DR_CHANNELMASK_BACK_RIGHT),
      DR_CHANNELMASK_7POINT1 = (DR_CHANNELMASK_FRONT_LEFT | DR_CHANNELMASK_FRONT_RIGHT | DR_CHANNELMASK_FRONT_CENTER | DR_CHANNELMASK_LOW_FREQUENCY | DR_CHANNELMASK_SURROUND_LEFT | DR_CHANNELMASK_SURROUND_RIGHT | DR_CHANNELMASK_BACK_LEFT | DR_CHANNELMASK_BACK_RIGHT)
    };
  };

  struct DR_SOUND_EXPORT DrFileOpenCallback {
    const char *name;
    UInt32 *filesize;
    void **handle;
    void *userdata;
  };
  
  struct DR_SOUND_EXPORT DrFileCloseCallback {
    void *handle;
    void *userdata;
  };

  struct DR_SOUND_EXPORT DrFileReadCallback {
    void *handle;
    void *buffer;
    unsigned int sizebytes;
    unsigned int *bytesread;
    void *userdata;
  };

  struct DR_SOUND_EXPORT DrFileSeekCallback {
    void *handle;
    unsigned int pos;
    void *userdata;
  };

  struct DR_SOUND_EXPORT DrGuid {
    UInt32   Data1;
    unsigned short Data2;
    unsigned short Data3; 
    unsigned char  Data4[8];
  };

  struct DR_SOUND_EXPORT DrCreateSoundExInfo {
    Int32 cbsize;
    UInt32 length;
    UInt32 fileoffset;
    Int32 numchannels;
    Int32 defaultfrequency;
    DR_SOUND_FORMAT::E format;
    UInt32 decodebuffersize;
    Int32 initialsubsound;
    Int32 numsubsounds;
    Int32 *inclusionlist;
    Int32 inclusionlistnum;
    FMOD_SOUND_PCMREAD_CALLBACK pcmreadcallback;
    FMOD_SOUND_PCMSETPOS_CALLBACK pcmsetposcallback;
    FMOD_SOUND_NONBLOCK_CALLBACK nonblockcallback;
    const char *dlsname;
    const char *encryptionkey;
    Int32 maxpolyphony;
    void *userdata;
    DR_SOUND_TYPE::E suggestedsoundtype;
    DrFileOpenCallback fileuseropen;
    DrFileCloseCallback fileuserclose;
    DrFileReadCallback fileuserread;
    DrFileSeekCallback fileuserseek;
    FMOD_FILE_ASYNCREAD_CALLBACK fileuserasyncread;
    FMOD_FILE_ASYNCCANCEL_CALLBACK fileuserasynccancel;
    void *fileuserdata;
    Int32 filebuffersize;
    DR_CHANNELORDER::E channelorder;
    DR_CHANNELMASK::E channelmask;
    FMOD_SOUNDGROUP *initialsoundgroup;
    UInt32 initialseekposition;
    DR_TIMEUNIT::E initialseekpostype;
    Int32 ignoresetfilesystem;
    UInt32 audioqueuepolicy;
    UInt32 minmidigranularity;
    Int32 nonblockthreadid;
    DrGuid *fsbguid;
  };
}