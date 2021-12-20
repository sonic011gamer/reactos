/*
 * PROJECT:     ReactOS APFS driver
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     Core APFS header file
 * COPYRIGHT:   Copyright 2021 Justin Miller <justinmiller100@gmail.com>
 */

#ifndef APFS_H
#define APFS_H

/* CONSTANTS *************************************************************************************/

/* Object Identifier Constants */
#define OID_NX_SUPERBLOCK               1;
#define OID_INVALID                     0ULL;
#define OID_RESERVED_COUNT              1024;

#define OBJECT_TYPE_MASK                0x0000FFFF;
#define OBJECT_TYPE_FLAGS_MASK          0xFFFF0000;
#define OBJ_STORAGETYPE_MASK            0xC0000000;
#define OBJECT_TYPE_FLAGS_DEFINED_MASK  0xF8000000;

/* Object Type Constants */
#define OBJECT_TYPE_NX_SUPERBLOCK       0x00000001;

#define OBJECT_TYPE_BTREE               0x00000002;
#define OBJECT_TYPE_BTREE_NODE          0x00000003;

#define OBJECT_TYPE_SPACEMAN            0x00000005;
#define OBJECT_TYPE_SPACEMAN_CAB        0x00000006;
#define OBJECT_TYPE_SPACEMAN_CIB        0x00000007;
#define OBJECT_TYPE_SPACEMAN_BITMAP     0x00000008;
#define OBJECT_TYPE_SPACEMAN_FREE_QUEUE 0x00000009;

#define OBJECT_TYPE_EXTENT_LIST_TREE    0x0000000A;
#define OBJECT_TYPE_OMAP                0x0000000B;
#define OBJECT_TYPE_CHECKPOINT_MAP      0x0000000C;

#define OBJECT_TYPE_FS                  0x0000000D;
#define OBJECT_TYPE_FSTREE              0x0000000E;
#define OBJECT_TYPE_BLOCKREFTREE        0x0000000F;
#define OBJECT_TYPE_SNAPMETATREE        0x00000010;

#define OBJECT_TYPE_NX_REAPER           0x00000011;
#define OBJECT_TYPE_NX_REAP_LIST        0x00000012;
#define OBJECT_TYPE_OMAP_SNAPSHOT       0x00000013;
#define OBJECT_TYPE_EFI_JUMPSTART       0x00000014;
#define OBJECT_TYPE_FUSION_MIDDLE_TREE  0x00000015;
#define OBJECT_TYPE_NX_FUSION_WBC       0x00000016;
#define OBJECT_TYPE_NX_FUSION_WBC_LIST  0x00000017;
#define OBJECT_TYPE_ER_STATE            0x00000018;

#define OBJECT_TYPE_GBITMAP             0x00000019;
#define BJECT_TYPE_GBITMAP_TREE         0x0000001A;
#define OBJECT_TYPE_GBITMAP_BLOCK       0x0000001B;

#define OBJECT_TYPE_ER_RECOVERY_BLOCK   0x0000001C;
#define OBJECT_TYPE_SNAP_META_EXT       0x0000001D;
#define OBJECT_TYPE_INTEGRITY_META      0x0000001E;
#define OBJECT_TYPE_FEXT_TREE           0x0000001F;
#define OBJECT_TYPE_RESERVED_20         0x00000020;

/* Object Type Flags */
#define OBJ_VIRTUAL                     0x00000000;
#define OBJ_EPHEMERAL                   0x80000000;
#define OBJ_PHYSICAL                    0x40000000;

#define OBJ_NOHEADER                    0x20000000;
#define OBJ_ENCRYPTED                   0x10000000;
#define OBJ_NONPERSISTENT               0x08000000;

/* UEFI Boot Constants */
#define NX_EFI_JUMPSTART_MAGIC          "RDSJ";
#define NX_EFI_JUMPSTART_VERSION        1;

#define APFS_GPT_PARTITION_UUID         "7C3457EF-0000-11AA-AA11-00306543ECAC”;

#define NX_MAGIC                        "BSXN";
#define NX_MAX_FILE_SYSTEMS             100;
#define NX_EPH_INFO_COUNT               4;
#define NX_EPH_MIN_BLOCK_COUNT          8;
#define NX_MAX_FILE_SYSTEM_EPH_STRUCTS  4;
#define NX_TX_MIN_CHECKPOINT_COUNT      4;
#define NX_EPH_INFO_VERSION_1           1;

/* Container Flags */
#define NX_RESERVED_1                   0x00000001LL;
#define NX_RESERVED_2                   0x00000002LL;
#define NX_CRYPTO_SW                    0x00000004LL;

/* Optional Container Flags */
#define NX_FEATURE_DEFRAG               0x0000000000000001ULL;
#define NX_FEATURE_LCFD                 0x0000000000000002ULL;
#define NX_SUPPORTED_FEATURES_MASK      (NX_FEATURE_DEFRAG | NX_FEATURE_LCFD);

/* Incompatible Container Flags */
#define NX_INCOMPAT_VERSION1            0x0000000000000001ULL;
#define NX_INCOMPAT_VERSION2            0x0000000000000002ULL;
#define NX_INCOMPAT_FUSION              0x0000000000000100ULL;
#define NX_SUPPORTED_INCOMPAT_MASK      (NX_INCOMPAT_VERSION2 | NX_INCOMPAT_FUSION);

/* Block and container sizes */
#define NX_MINIMUM_BLOCK_SIZE           4096;
#define NX_DEFAULT_BLOCK_SIZE           4096;
#define NX_MAXIMUM_BLOCK_SIZE           65536;
#define NX_MINIMUM_CONTAINER_SIZE       1048576;

/* Object map value falgs */
#define OMAP_VAL_DELETED                0x00000001;
#define OMAP_VAL_SAVED                  0x00000002;
#define OMAP_VAL_ENCRYPTED              0x00000004;
#define OMAP_VAL_NOHEADER               0x00000008;
#define OMAP_VAL_CRYPTO_GENERATION      0x00000010;

/* Snapshot flags */
#define OMAP_SNAPSHOT_DELETED           0x00000001;
#define OMAP_SNAPSHOT_REVERTED          0x00000002;

/* Object Map Flags */
#define OMAP_MANUALLY_MANAGED           0x00000001;
#define OMAP_ENCRYPTING                 0x00000002;
#define OMAP_DECRYPTING                 0x00000004;
#define OMAP_KEYROLLING                 0x00000008;
#define OMAP_CRYPTO_GENERATION          0x00000010;
#define OMAP_VALID_FLAGS                0x0000001f;

#define APFS_MAGIC                      "BSPA";
#define APFS_MAX_HIST                   8;
#define APFS_VOLNAME_LEN                256;
#define APFS_MODIFIED_NAMELEN           32;

/* Volume Flags */
#define APFS_FS_UNENCRYPTED             0x00000001LL;
#define APFS_FS_RESERVED_2              0x00000002LL;
#define APFS_FS_RESERVED_4              0x00000004LL;
#define APFS_FS_ONEKEY                  0x00000008LL;
#define APFS_FS_SPILLEDOVER             0x00000010LL;
#define APFS_FS_RUN_SPILLOVER_CLEANER   0x00000020LL;
#define APFS_FS_ALWAYS_CHECK_EXTENTREF  0x00000040LL;
#define APFS_FS_RESERVED_80             0x00000080LL;
#define APFS_FS_RESERVED_100            0x00000100LL;

#define APFS_FS_FLAGS_VALID_MASK        (APFS_FS_UNENCRYPTED \
                                        | APFS_FS_RESERVED_2 \
                                        | APFS_FS_RESERVED_4 \
                                        | APFS_FS_ONEKEY \
                                        | APFS_FS_SPILLEDOVER \
                                        | APFS_FS_RUN_SPILLOVER_CLEANER \
                                        | APFS_FS_ALWAYS_CHECK_EXTENTREF \
                                        | APFS_FS_RESERVED_80 \
                                        | APFS_FS_RESERVED_100);

#define APFS_FS_CRYPTOFLAGS             (APFS_FS_UNENCRYPTED \
                                        | APFS_FS_ONEKEY);

/* Volume Roles */
#define APFS_VOL_ROLE_NONE              0x0000;
#define APFS_VOL_ROLE_SYSTEM            0x0001;
#define APFS_VOL_ROLE_USER              0x0002;
#define APFS_VOL_ROLE_RECOVERY          0x0004;
#define APFS_VOL_ROLE_VM                0x0008;
#define APFS_VOL_ROLE_PREBOOT           0x0010;
#define APFS_VOL_ROLE_INSTALLER         0x0020;

#define APFS_VOL_ROLE_DATA              (1 << APFS_VOLUME_ENUM_SHIFT);
#define APFS_VOL_ROLE_BASEBAND          (2 << APFS_VOLUME_ENUM_SHIFT);
#define APFS_VOL_ROLE_UPDATE            (3 << APFS_VOLUME_ENUM_SHIFT);
#define APFS_VOL_ROLE_XART              (4 << APFS_VOLUME_ENUM_SHIFT);
#define APFS_VOL_ROLE_HARDWARE          (5 << APFS_VOLUME_ENUM_SHIFT);
#define APFS_VOL_ROLE_BACKUP            (6 << APFS_VOLUME_ENUM_SHIFT);
#define APFS_VOL_ROLE_RESERVED_7        (7 << APFS_VOLUME_ENUM_SHIFT);
#define APFS_VOL_ROLE_RESERVED_8        (8 << APFS_VOLUME_ENUM_SHIFT);
#define APFS_VOL_ROLE_ENTERPRISE        (9 << APFS_VOLUME_ENUM_SHIFT);
#define APFS_VOL_ROLE_RESERVED_10       (10 << APFS_VOLUME_ENUM_SHIFT);
#define APFS_VOL_ROLE_PRELOGIN          (11 << APFS_VOLUME_ENUM_SHIFT);

#define APFS_VOLUME_ENUM_SHIFT          6;

/* Optional Volume Feature Flags - pg 67*/
#define APFS_FEATURE_DEFRAG_PRERELEASE       0x00000001LL;
#define APFS_FEATURE_HARDLINK_MAP_RECORDS    0x00000002LL;
#define APFS_FEATURE_DEFRAG                  0x00000004LL;
#define APFS_FEATURE_STRICTATIME             0x00000008LL;
#define APFS_FEATURE_VOLGRP_SYSTEM_INO_SPACE 0x00000010LL;

// LEFT OFF PAGE 67
/* GENERAL-PURPOSE TYPES *************************************************************************/

typedef INT64 PADDR
typedef UCHAR UUID[16];
typedef UINT64 OID;
typedef UINT64 XID;

/* A range of a physical addresses */
struct _PRANGE {
    PADDER PRStartPAdder; /* First block in a range */
    UINT64 PRBlockCount;    /* Number of blocks in a range */
} PRANGE, *PPRANGE;

/* Header used at the beginning of all objects */
struct _OBJ_PHYS {
    UINT8 O_Chksum[MAX_CHKSUM_SIZE]
    OID OOId;
    XID OXId;
    UINT32 OType
    UINT32 OSubtype
} OBJ_PHYS, *POBJ_PHYS

struct _NX_EFI_JUMPSTART {
    OBJ_PHYS NejO;
    UINT32 NejMagic;
    UINT32 NejVersion;
    UINT32 NejEfiFileLen;
    UINT32 NejNumExtents;
    UINT64 NejReserved[16];
    PRange NejRecExtents[];
} NX_EFI_JUMPSTART, *PNX_EFI_JUMPSTART;

struct _NX_SUPERBLOCK {
    OBJ_PHYS Nx_O;
    UINT32 NxMagic;
    UINT32 NxBlockSize;
    UINT64 NxBlockCount;

    UINT64 NxFeatures;
    UINT64 NxReadOnlyCompatibleFeatures;
    UINT64 NxIncompatibleFeatures;

    UUID NxUuid;

    OID NxNextOId;
    XID NxNextXId;

    UINT32 NxXpDescBlocks;
    UINT32 NxXpDataBlocks;
    PADDR NxXpDescBase;
    PADDR NxXpDataBase;
    UINT32 NxXpDescNext;
    UINT32 NxXpDataNext;
    UINT32 NxXpDescIndex;
    UINT32 NxXpDescLen;
    UINT32 NxXpDataIndex;
    UINT32 NxXpDataLen;

    OID NxSpacemanOId;
    OID NxOmapOId;
    OID NxReaperOId;

    UINT32 NxTestType;

    UINT32 NxMaxFileSystem;
    OID NxFsOId[NX_MAX_FILE_SYSTEMS];
    UINT64 NxCounters[NX_NUM_COUNTERS];
    PRANGE NxBlockedOutPRange;
    OID NxEvictMappingTreeOld;
    UINT64 NxFlags;
    PADDER NxEfiJumpstart;
    UUID NxFusionUuid;
    PRANGE NxKeylocker;
    UINT64 NxEphemeralInfo[NX_EPH_INFO_COUNT];

    OID NxTestOId;
    OID NxFusionMtOId;
    OID NxFusionWbcOId;
    PRANGE NxFusionWbc;
    UINT64 NxNewestMountedVersion;
    PRANGE NxMkbLocker;
} NX_SUPERBLOCK, *PNX_SUPERBLOCK;

typedef enum _NX_COUNTER_ID {
    NX_CNTR_OBJ_CKSUM_SET = 0,
    NX_CNTR_OBJ_CKSUM_FAIL = 1,
    NX_NUM_COUNTERS = 32
} NX_COUNTER_ID;

struct _CHECKPOINT_MAPPING {
    UINT32 CpmType;
    UINT32 CpmSubtype;
    UINT32 CpmSize;
    UINT32 CpmPad;
    OID CpmFsOld;
    OID CpmOId;
    OID CpmPaddr
} CHECKPOINT_MAPPING;

struct _CHECKPOINT_MAP_PHY {
    OBJ_PHYS CpmO;
    UINT32 CpmFlags;
    UINT32 CpmCount;
    CHECKPOINT_MAPPING CpmMap[];
} CHECKPOINT_MAP_PHY, *PCHECKPOINT_MAP_PHY;

#include <poppack>
struct _EVICT_MAPPIMG_VAL {
    PADDR DstPAddr;
    UINT64 Len;
} EVICT_MAPPIMG_VAL;
#include <pshpack>

struct _OMAP_PHYS {
    OBJ_PHYS OmO;
    UINT32 OmFalgs;
    UINT32 OmSnapCount;
    UINT32 OmTreeType;
    UINT32 OmSnapshotTreeType;
    OID OmTreeOld;
    OID OmSnapshotTreeOId;
    XID OmMostRecentSnap;
    XID OmPendingRevertMin;
    XID OmPendingRevertMax;
} OMAP_PHYS, *POMAP_PHYS;

struct _OMAP_VAL {
    UINT32 OvFlags;
    UINT32 OvSize;
    PADDR OvPadder;
} OMAP_VAL, *POMAP_VAL;

struct _OMAP_SNAPSHOT {
    UINT32 OmsFlags;
    UINT32 OmsPad;
    OID OmsOId
} OMAP_SNAPSHOT, *POMAP_SNAPSHOP;

struct _APFS_MODIFIED_BY {
    UINT8 Id[APFS_MODIFIED_NAMELEN];
    UINT64 TimeStamp;
    XID LastXId;
} APFS_MODIFIED_BY, *PAPFS_MODIFIED_BY;

struct _APFS_SUPERBLOCK {
    OBJ_PHYS ApfsO;
    UINT32 ApfsMagic;
    UINT32 ApfsFsIndex;

    UINT64 ApfsFeatures;
    UINT64 ApfsReadonlyCompatibleFeatures;
    UINT64 ApfsIncompatibleFeatures;

    WRAPPED_META_CRYPTO_STATE ApfsMetaCrypto;

    UINT32 ApfsRootTreeType;
    UINT32 ApfsExtentRefTreeType;
    UINT32 ApfsSnapMetaTreeType;

    OID ApfsOmapOld;
    OID ApfsRootTreeOId;
    OID ApfsExtendRefTreeOld;
    OID ApfsSnapMetaTreeType;

    XID ApfsRevertToXId;
    OID ApfsRevertToSBlockOld;

    UINT64 ApfsNextObjId;

    UINT64 ApfsNumFiles;
    UINT64 ApfsNumDirectories;
    UINT64 ApfsNumSymlinks;
    UINT64 ApfsNumOtherFSObjects;
    UINT64 ApfsNumSnapshots;
    UINT64 ApfsTotalBlocksAllocated;
    UINT64 ApfsTotalBlocksFreed;

    UUID ApfsVolUuid;
    UINT64 ApfsLastModTime;

    UINT64 ApfsFsFlags;

    APFS_MODIFIED_BY ApfsFormattedBy;
    APFS_MODIFIED_BY ApfsModifiedBy[APFS_MAX_HIST];

    UINT8 ApfsVolnamel[APFS_VOONAME_LEN];
    UINT32 ApfsNextDocId;

    UINT16 ApfsRole;
    UINT16 ApfsErStateOId;

    OID ApfsSnapMetaExtOId;
    UUID ApfsVolumeGroupId;
    OID ApfsIntegrityMetaOId;

    OID ApfsFExtTreeOId;
    UINT32 ApfsFExtTreeType;

    UINT32 ReservedType;
    OID ReservedOId;
} APFS_SUPERBLOCK, *PAPFS_SUPERBLOCK;

#endif /* APFS_H */
