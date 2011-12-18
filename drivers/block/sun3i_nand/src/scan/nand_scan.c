/*
************************************************************************************************************************
*                                                      eNand
*                                           Nand flash driver scan module
*
*                             Copyright(C), 2008-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name : nand_scan.c
*
* Author : Kevin.z
*
* Version : v0.1
*
* Date : 2008.03.27
*
* Description : This file scan the nand flash storage system, analyze the nand flash type
*               and initiate the physical architecture parameters.
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
* Kevin.z         2008.03.27      0.1          build the file
*
************************************************************************************************************************
*/

#include "../include/nand_scan.h"
#include"../../nfc/nfc.h"


extern  struct __NandStorageInfo_t  NandStorageInfo;

extern struct __NandPhyInfoPar_t SamsungNandTbl;
extern struct __NandPhyInfoPar_t HynixNandTbl;
extern struct __NandPhyInfoPar_t ToshibaNandTbl;
extern struct __NandPhyInfoPar_t MicronNandTbl;
extern struct __NandPhyInfoPar_t IntelNandTbl;
extern struct __NandPhyInfoPar_t StNandTbl;
extern struct __NandPhyInfoPar_t DefaultNandTbl;
extern struct __NandPhyInfoPar_t SpansionNandTbl;

__s32 NAND_Detect(_nand_connect_info_t *nand_connect);

/*
************************************************************************************************************************
*                           GENERAGE THE NAND PHYSICAL ARCHITECTURE PARAMETER
*
*Description: Generate the nand physical architecture paramter with the nand chip ID.
*
*Arguments  : pNandID           the pointer to nand flash chip ID;
*             pNandArchiInfo    the pointer to nand flash physical architecture parameter.
*
*Return     : Generate result;
*               = 0     Generate successful, create the parameter in a table;
*               < 0     Generate failed, can't create the parameter in a table.
************************************************************************************************************************
*/
__s32 _GenNandArchi(__u8 *pNandID, struct __NandPhyInfoPar_t *pNandArchInfo)
{
    __u32 tmpVar;
    struct __NandPhyInfoPar_t *tmpNandArchi = &DefaultNandTbl;

    //check the nand flash manufacture
    if((pNandID[0] != TOSHIBA_NAND) && (pNandID[0] != SAMSUNG_NAND) && (pNandID[0] != TOSHIBA_NAND) \
        && (pNandID[0] != HYNIX_NAND) && (pNandID[0] != MICRON_NAND) && (pNandID[0] != ST_NAND) \
        && (pNandID[0] != INTEL_NAND))
    {
        //don't recognize the nand manufacture, don't know how to analyze the nand chip ID
        SCAN_ERR("[SCAN_ERR] Can't recognize the nand flash manufacture[0x%x]!\n", pNandID[0]);
        return -1;
    }

    //create the physical architecture parameter in the default nand table
    tmpNandArchi->NandID[0] = pNandID[0];
    tmpNandArchi->NandID[1] = pNandID[1];
    tmpNandArchi->NandID[2] = pNandID[2];
    tmpNandArchi->NandID[3] = pNandID[3];

    //analyze the chip capacity
    switch(pNandID[1])
    {
        case 0xf1:
            tmpVar = 1;     //the capacity of the nand chip is 1G bit
            break;

        case 0xda:
            tmpVar = 2;     //the capacity of the nand chip is 1G bit
            break;

        case 0xdc:
            tmpVar = 4;     //the capacity of the nand chip is 1G bit
            break;

        case 0xd3:
            tmpVar = 8;     //the capacity of the nand chip is 1G bit
            break;

        case 0xd5:
            tmpVar = 16;    //the capacity of the nand chip is 1G bit
            break;

        case 0xd7:
            tmpVar = 32;    //the capacity of the nand chip is 1G bit
            break;

        default:            //don't recognize the nand chip capacity
            return -1;
    }

    //parse the die count of a chip
    tmpNandArchi->DieCntPerChip = 1<<((pNandID[2]>>0) & 0x03);

    //parse the sector count of a physical page
    tmpNandArchi->SectCntPerPage = 2<<((pNandID[3]>>0) & 0x03);

    //parse the page count of a physical block
    tmpNandArchi->PageCntPerBlk = ((1<<((pNandID[3]>>4) & 0x03)) * 64 * 2) / tmpNandArchi->SectCntPerPage;

    //parse the physical block count of a die
    tmpNandArchi->BlkCntPerDie = (((__u32)(tmpVar * 1024 * 1024 / 8)) /  \
        ((1<<((pNandID[3]>>4) & 0x03)) * 64)) / tmpNandArchi->DieCntPerChip;

    //parse the bad block flag position, normaly, first page for SLC nand, last page for MLC nand flash
    if((2<<((pNandID[2]>>2) & 0x03)) > 2)
    {
        //current nand flash is MLC type nand flash
        if((tmpNandArchi->NandID[0] == MICRON_NAND) || (tmpNandArchi->NandID[0] == INTEL_NAND))
        {
            //the bad block flag for Micron and Intel MLC nand flash is in first page
            tmpNandArchi->OptionOp->BadBlockFlagPosition = 0;
        }
        else
        {
            //default is in the last page
             tmpNandArchi->OptionOp->BadBlockFlagPosition = 2;
        }
    }
    else
    {
        //current nand flash is SLC type nand flash, the bad block flag is in the first page
        tmpNandArchi->OptionOp->BadBlockFlagPosition = 0;
    }

    //can't parse if the nand flash support multi-plane or inter-leave ex. operation
    tmpNandArchi->OperationOpt = 0;

    //set the nand flash access frequence parameter, default is 20MHz
    tmpNandArchi->AccessFreq = 20;

	//set the nand flash access EccMode, default is bch16
    tmpNandArchi->EccMode= 0;

    //set the pointer to the physical architectrue parameter for return
    //pNandArchInfo = tmpNandArchi;
    MEMCPY(pNandArchInfo,tmpNandArchi,sizeof(struct __NandPhyInfoPar_t));

    //analyze nand physical architecture parameter successful
    return 0;
}


/*
************************************************************************************************************************
*                           SEARCH NAND PHYSICAL ARCHITECTURE PARAMETER
*
*Description: Search the nand flash physical architecture parameter from the parameter table
*             by nand chip ID.
*
*Arguments  : pNandID           the pointer to nand flash chip ID;
*             pNandArchiInfo    the pointer to nand flash physical architecture parameter.
*
*Return     : search result;
*               = 0     search successful, find the parameter in the table;
*               < 0     search failed, can't find the parameter in the table.
************************************************************************************************************************
*/
__s32 _SearchNandArchi(__u8 *pNandID, struct __NandPhyInfoPar_t *pNandArchInfo)
{
    __s32 i=0, j=0;
    struct __NandPhyInfoPar_t *tmpNandManu;

    //analyze the manufacture of the nand flash
    switch(pNandID[0])
    {
        //manufacture is Samsung, search parameter from Samsung nand table
        case SAMSUNG_NAND:
            tmpNandManu = &SamsungNandTbl;
            break;

        //manufacture is Hynix, search parameter from Hynix nand table
        case HYNIX_NAND:
            tmpNandManu = &HynixNandTbl;
            break;

        //manufacture is Micron, search parameter from Micron nand table
        case MICRON_NAND:
            tmpNandManu = &MicronNandTbl;
            break;

        //manufacture is Intel, search parameter from Intel nand table
        case INTEL_NAND:
            tmpNandManu = &IntelNandTbl;
            break;

        //manufacture is Toshiba, search parameter from Toshiba nand table
        case TOSHIBA_NAND:
            tmpNandManu = &ToshibaNandTbl;
            break;

        //manufacture is St, search parameter from St nand table
        case ST_NAND:
            tmpNandManu = &StNandTbl;
            break;

		//manufacture is Spansion, search parameter from Spansion nand table
        case SPANSION_NAND:
            tmpNandManu = &SpansionNandTbl;
            break;
        //manufacture is unknown, search parameter from default nand table
        default:
            tmpNandManu = &DefaultNandTbl;
            break;
    }

    //search the nand architecture parameter from the given manufacture nand table by nand ID
    while(tmpNandManu[i].NandID[0] != 0xff)
    {
        for(j=1; j<4; j++)
        {
            //0xff is matching all ID value
            if((pNandID[j] != tmpNandManu[i].NandID[j]) && (tmpNandManu[i].NandID[j] != 0xff))
            break;
        }

        if(j==4)
        {
           /*4 bytes of the nand chip ID are all matching, search parameter successful*/
            //pNandArchInfo = tmpNandManu+i;
            MEMCPY(pNandArchInfo,tmpNandManu+i,sizeof(struct __NandPhyInfoPar_t));
            return 0;
        }

        //prepare to search the next table item
        i++;
    }

    //search nand architecture parameter failed
    return -1;
}


/*
************************************************************************************************************************
*                           ANALYZE NAND FLASH STORAGE SYSTEM
*
*Description: Analyze nand flash storage system, generate the nand flash physical
*             architecture parameter and connect information.
*
*Arguments  : none
*
*Return     : analyze result;
*               = 0     analyze successful;
*               < 0     analyze failed, can't recognize or some other error.
************************************************************************************************************************
*/
__s32  SCN_AnalyzeNandSystem(void)
{
    __s32 i,result;
    __u8  tmpChipID[8];
    struct __NandPhyInfoPar_t tmpNandPhyInfo;

    //init nand flash storage information to default value
    NandStorageInfo.ChipCnt = 1;
    NandStorageInfo.ChipConnectInfo = 1;
    NandStorageInfo.RbConnectMode= 1;
    NandStorageInfo.RbCnt= 1;
    NandStorageInfo.RbConnectInfo= 1;
    NandStorageInfo.BankCntPerChip = 1;
    NandStorageInfo.DieCntPerChip = 1;
    NandStorageInfo.PlaneCntPerDie = 1;
    NandStorageInfo.SectorCntPerPage = 4;
    NandStorageInfo.PageCntPerPhyBlk = 64;
    NandStorageInfo.BlkCntPerDie = 1024;
    NandStorageInfo.OperationOpt = 0;
    NandStorageInfo.FrequencePar = 10;
    NandStorageInfo.EccMode = 0;

    //reset the nand flash chip on boot chip select
    result = PHY_ResetChip(BOOT_CHIP_SELECT_NUM);
    result |= PHY_SynchBank(BOOT_CHIP_SELECT_NUM, SYNC_CHIP_MODE);
    if(result)
    {
        SCAN_ERR("[SCAN_ERR] Reset boot nand flash chip failed!\n");
        return -1;
    }

    //read nand flash chip ID from boot chip
    result = PHY_ReadNandId(BOOT_CHIP_SELECT_NUM, tmpChipID);
    if(result)
    {
        SCAN_ERR("[SCAN_ERR] Read chip ID from boot chip failed!\n");
        return -1;
    }
    SCAN_DBG("[SCAN_DBG] Nand flash chip id is:0x%x 0x%x 0x%x 0x%x\n",
            tmpChipID[0],tmpChipID[1],tmpChipID[2],tmpChipID[3]);

    //search the nand flash physical architecture parameter by nand ID
    result = _SearchNandArchi(tmpChipID, &tmpNandPhyInfo);
    if(result)
    {
        //search nand flash physical architecture parameter failed, need generate them by chip ID
        result = _GenNandArchi(tmpChipID, &tmpNandPhyInfo);
        if(result)
        {
            //Generate nand flash physical archittecture parameter failed
            SCAN_ERR("[SCAN_ERR] Generate nand physical architecture parameter failed!\n");
            return -1;
        }

        SCAN_DBG("[SCAN_DBG] Search nand physical archtecture parameter failed, but generate successful.\n");
    }

    //set the nand flash physical architecture parameter
    NandStorageInfo.BankCntPerChip = tmpNandPhyInfo.DieCntPerChip;
    NandStorageInfo.DieCntPerChip = tmpNandPhyInfo.DieCntPerChip;
    NandStorageInfo.PlaneCntPerDie = 2;
    NandStorageInfo.SectorCntPerPage = tmpNandPhyInfo.SectCntPerPage;
    NandStorageInfo.PageCntPerPhyBlk = tmpNandPhyInfo.PageCntPerBlk;
    NandStorageInfo.BlkCntPerDie = tmpNandPhyInfo.BlkCntPerDie;
    NandStorageInfo.OperationOpt = tmpNandPhyInfo.OperationOpt;
    NandStorageInfo.FrequencePar = tmpNandPhyInfo.AccessFreq;
    NandStorageInfo.EccMode = tmpNandPhyInfo.EccMode;
    NandStorageInfo.NandChipId[0] = tmpNandPhyInfo.NandID[0];
    NandStorageInfo.NandChipId[1] = tmpNandPhyInfo.NandID[1];
    NandStorageInfo.NandChipId[2] = tmpNandPhyInfo.NandID[2];
    NandStorageInfo.NandChipId[3] = tmpNandPhyInfo.NandID[3];
    NandStorageInfo.ValidBlkRatio = tmpNandPhyInfo.ValidBlkRatio;
    //set the optional operation parameter
    NandStorageInfo.OptPhyOpPar.MultiPlaneReadCmd[0] = tmpNandPhyInfo.OptionOp->MultiPlaneReadCmd[0];
    NandStorageInfo.OptPhyOpPar.MultiPlaneReadCmd[1] = tmpNandPhyInfo.OptionOp->MultiPlaneReadCmd[1];
    NandStorageInfo.OptPhyOpPar.MultiPlaneWriteCmd[0] = tmpNandPhyInfo.OptionOp->MultiPlaneWriteCmd[0];
    NandStorageInfo.OptPhyOpPar.MultiPlaneWriteCmd[1] = tmpNandPhyInfo.OptionOp->MultiPlaneWriteCmd[1];
    NandStorageInfo.OptPhyOpPar.MultiPlaneCopyReadCmd[0] = tmpNandPhyInfo.OptionOp->MultiPlaneCopyReadCmd[0];
    NandStorageInfo.OptPhyOpPar.MultiPlaneCopyReadCmd[1] = tmpNandPhyInfo.OptionOp->MultiPlaneCopyReadCmd[1];
    NandStorageInfo.OptPhyOpPar.MultiPlaneCopyReadCmd[2] = tmpNandPhyInfo.OptionOp->MultiPlaneCopyReadCmd[2];
    NandStorageInfo.OptPhyOpPar.MultiPlaneCopyWriteCmd[0] = tmpNandPhyInfo.OptionOp->MultiPlaneCopyWriteCmd[0];
    NandStorageInfo.OptPhyOpPar.MultiPlaneCopyWriteCmd[1] = tmpNandPhyInfo.OptionOp->MultiPlaneCopyWriteCmd[1];
    NandStorageInfo.OptPhyOpPar.MultiPlaneCopyWriteCmd[2] = tmpNandPhyInfo.OptionOp->MultiPlaneCopyWriteCmd[2];
    NandStorageInfo.OptPhyOpPar.MultiPlaneStatusCmd = tmpNandPhyInfo.OptionOp->MultiPlaneStatusCmd;
    NandStorageInfo.OptPhyOpPar.InterBnk0StatusCmd = tmpNandPhyInfo.OptionOp->InterBnk0StatusCmd;
    NandStorageInfo.OptPhyOpPar.InterBnk1StatusCmd = tmpNandPhyInfo.OptionOp->InterBnk1StatusCmd;
    NandStorageInfo.OptPhyOpPar.BadBlockFlagPosition = tmpNandPhyInfo.OptionOp->BadBlockFlagPosition;
    NandStorageInfo.OptPhyOpPar.MultiPlaneBlockOffset = tmpNandPhyInfo.OptionOp->MultiPlaneBlockOffset;

    //set some configurable  optional operation parameter
    if(!CFG_SUPPORT_MULTI_PLANE_PROGRAM)
    {
        NandStorageInfo.OperationOpt &= ~NAND_MULTI_READ;
        NandStorageInfo.OperationOpt &= ~NAND_MULTI_PROGRAM;
    }

    if(!CFG_SUPPORT_INT_INTERLEAVE)
    {
        NandStorageInfo.OperationOpt &= ~NAND_INT_INTERLEAVE;
    }

    //process the plane count of a die and the bank count of a chip
    if(!SUPPORT_MULTI_PROGRAM)
    {
        NandStorageInfo.PlaneCntPerDie = 1;
    }

    if(!SUPPORT_INT_INTERLEAVE)
    {
        NandStorageInfo.BankCntPerChip = 1;
    }
   #if(0)
    //process the nand flash chip connected information
    for(i=1; i<MAX_CHIP_SELECT_CNT; i++)
    {
        //reset current nand flash chip
        __u16 ChipConnectInfo = NandStorageInfo.ChipConnectInfo;
		  NandStorageInfo.ChipConnectInfo = 0xff;
        PHY_ResetChip((__u32)i);
		
   
        if(PHY_SynchBank((__u32)(NandStorageInfo.BankCntPerChip * i), 0))
		  {	
		  		NandStorageInfo.ChipConnectInfo = ChipConnectInfo;
		  		continue;
        }
		  NandStorageInfo.ChipConnectInfo = ChipConnectInfo;
    
	
        //read the nand chip ID from current nand flash chip
        PHY_ReadNandId((__u32)i, tmpChipID);
        //check if the nand flash id same as the boot chip
        if((tmpChipID[0] == NandStorageInfo.NandChipId[0]) && (tmpChipID[1] == NandStorageInfo.NandChipId[1])
            && (tmpChipID[2] == NandStorageInfo.NandChipId[2]) && (tmpChipID[3] == NandStorageInfo.NandChipId[3]))
        {
            NandStorageInfo.ChipCnt++;
            NandStorageInfo.ChipConnectInfo |= (1<<i);
        }
    }	
#elif (1)
     //process the rb connect infomation

     for(i=1; i<MAX_CHIP_SELECT_CNT; i++)
    {
        //reset current nand flash chip
        PHY_ResetChip((__u32)i);

        //read the nand chip ID from current nand flash chip
        PHY_ReadNandId((__u32)i, tmpChipID);
        //check if the nand flash id same as the boot chip
        if((tmpChipID[0] == NandStorageInfo.NandChipId[0]) && (tmpChipID[1] == NandStorageInfo.NandChipId[1])
            && (tmpChipID[2] == NandStorageInfo.NandChipId[2]) && (tmpChipID[3] == NandStorageInfo.NandChipId[3]))
        {
            NandStorageInfo.ChipCnt++;
            NandStorageInfo.ChipConnectInfo |= (1<<i);
        }
     	}

    //process the rb connect infomation
    {
        NandStorageInfo.RbConnectMode = 0xff;
		
        if((NandStorageInfo.ChipCnt == 1) && (NandStorageInfo.ChipConnectInfo & (1<<0)))
        {
             NandStorageInfo.RbConnectMode =1;
        }	     
        else if(NandStorageInfo.ChipCnt == 2)
        {
    	      if((NandStorageInfo.ChipConnectInfo & (1<<0)) && (NandStorageInfo.ChipConnectInfo & (1<<1)))
		    NandStorageInfo.RbConnectMode =2; 
	      else if((NandStorageInfo.ChipConnectInfo & (1<<0)) && (NandStorageInfo.ChipConnectInfo & (1<<2)))
		    NandStorageInfo.RbConnectMode =3; 	
		else if((NandStorageInfo.ChipConnectInfo & (1<<0)) && (NandStorageInfo.ChipConnectInfo & (1<<7)))
		    NandStorageInfo.RbConnectMode =0; 	//special use, only one rb 
		  
        }
		
        else if(NandStorageInfo.ChipCnt == 4)
        {
    	      if((NandStorageInfo.ChipConnectInfo & (1<<0)) && (NandStorageInfo.ChipConnectInfo & (1<<1)) 
			  	&&  (NandStorageInfo.ChipConnectInfo & (1<<2)) &&  (NandStorageInfo.ChipConnectInfo & (1<<3)) )
		    NandStorageInfo.RbConnectMode =4; 
	      else if((NandStorageInfo.ChipConnectInfo & (1<<0)) && (NandStorageInfo.ChipConnectInfo & (1<<2)) 
			  	&&  (NandStorageInfo.ChipConnectInfo & (1<<4)) &&  (NandStorageInfo.ChipConnectInfo & (1<<6)) )
		    NandStorageInfo.RbConnectMode =5; 			
        }
        else if(NandStorageInfo.ChipCnt == 8)
        {
	      NandStorageInfo.RbConnectMode =8; 			
        }
		
		if( NandStorageInfo.RbConnectMode == 0xff)
            {   
        	    SCAN_ERR("%s : check nand rb connect fail, ChipCnt =  %x, ChipConnectInfo = %x \n",__FUNCTION__, NandStorageInfo.ChipCnt, NandStorageInfo.ChipConnectInfo);
        	    return -1;
		}

	 
    }
#endif    
	
    //process the external inter-leave operation
    if(CFG_SUPPORT_EXT_INTERLEAVE)
    {
        if(NandStorageInfo.ChipCnt > 1)
        {
            NandStorageInfo.OperationOpt |= NAND_EXT_INTERLEAVE;
        }
    }
    else
    {
        NandStorageInfo.OperationOpt &= ~NAND_EXT_INTERLEAVE;
    }

	/*configure page size*/
	{
		NFC_INIT_INFO nand_info;
		nand_info.bus_width = 0x0;
		nand_info.ce_ctl = 0x0;
		nand_info.ce_ctl1 = 0x0;
		nand_info.debug = 0x0;
		nand_info.pagesize = SECTOR_CNT_OF_SINGLE_PAGE;	
		nand_info.rb_sel = 1;
		nand_info.serial_access_mode = 1;
		NFC_ChangMode(&nand_info);
	}

#if (0)
	/*detect rb connect*/
	{
		_nand_connect_info_t nand_connect;

		NAND_Detect(&nand_connect);

		if((nand_connect.id[0] != NandStorageInfo.NandChipId[0]) || (nand_connect.id[1] != NandStorageInfo.NandChipId[1])
            || (nand_connect.id[2] != NandStorageInfo.NandChipId[2]) || (nand_connect.id[3] != NandStorageInfo.NandChipId[3]))
        {   
        	SCAN_ERR("%s : check nand id fail\n",__FUNCTION__);
        	return -1;
		}
		
		NandStorageInfo.RbConnectInfo = nand_connect.rb_connect;
		NandStorageInfo.RbCnt= nand_connect.rb_cnt;
	}
#endif
	
    //print nand flash physical architecture parameter
    SCAN_DBG("\n\n");
    SCAN_DBG("[SCAN_DBG] ==============Nand Architecture Parameter==============\n");
    SCAN_DBG("[SCAN_DBG]    Nand Chip ID:         0x%x\n",
        (NandStorageInfo.NandChipId[0] << 0) | (NandStorageInfo.NandChipId[1] << 8)
        | (NandStorageInfo.NandChipId[2] << 16) | (NandStorageInfo.NandChipId[3] << 24));
    SCAN_DBG("[SCAN_DBG]    Nand Chip Count:      0x%x\n", NandStorageInfo.ChipCnt);
    SCAN_DBG("[SCAN_DBG]    Nand Chip Connect:    0x%x\n", NandStorageInfo.ChipConnectInfo);
	#if (0)
	SCAN_DBG("[SCAN_DBG]    Nand Rb Count:        0x%x\n", NandStorageInfo.RbCnt);
	SCAN_DBG("[SCAN_DBG]    Nand Rb Connect:      0x%x\n", NandStorageInfo.RbConnectInfo);
    #elif (1)
	SCAN_DBG("[SCAN_DBG]    Nand Rb Connect Mode:      0x%x\n", NandStorageInfo.RbConnectMode);
	#endif
    SCAN_DBG("[SCAN_DBG]    Sector Count Of Page: 0x%x\n", NandStorageInfo.SectorCntPerPage);
    SCAN_DBG("[SCAN_DBG]    Page Count Of Block:  0x%x\n", NandStorageInfo.PageCntPerPhyBlk);
    SCAN_DBG("[SCAN_DBG]    Block Count Of Die:   0x%x\n", NandStorageInfo.BlkCntPerDie);
    SCAN_DBG("[SCAN_DBG]    Plane Count Of Die:   0x%x\n", NandStorageInfo.PlaneCntPerDie);
    SCAN_DBG("[SCAN_DBG]    Die Count Of Chip:    0x%x\n", NandStorageInfo.DieCntPerChip);
    SCAN_DBG("[SCAN_DBG]    Bank Count Of Chip:   0x%x\n", NandStorageInfo.BankCntPerChip);
    SCAN_DBG("[SCAN_DBG]    Optional Operation:   0x%x\n", NandStorageInfo.OperationOpt);
    SCAN_DBG("[SCAN_DBG]    Access Frequence:     0x%x\n", NandStorageInfo.FrequencePar);
    SCAN_DBG("[SCAN_DBG]    ECC Mode:             0x%x\n", NandStorageInfo.EccMode);
    SCAN_DBG("[SCAN_DBG] =======================================================\n\n");

    //print nand flash optional operation parameter
    SCAN_DBG("[SCAN_DBG] ==============Optional Operaion Parameter==============\n");
    SCAN_DBG("[SCAN_DBG]    MultiPlaneReadCmd:      0x%x, 0x%x\n",
        NandStorageInfo.OptPhyOpPar.MultiPlaneReadCmd[0],NandStorageInfo.OptPhyOpPar.MultiPlaneReadCmd[1]);
    SCAN_DBG("[SCAN_DBG]    MultiPlaneWriteCmd:     0x%x, 0x%x\n",
        NandStorageInfo.OptPhyOpPar.MultiPlaneWriteCmd[0],NandStorageInfo.OptPhyOpPar.MultiPlaneWriteCmd[1]);
    SCAN_DBG("[SCAN_DBG]    MultiPlaneCopyReadCmd:  0x%x, 0x%x, 0x%x\n",
        NandStorageInfo.OptPhyOpPar.MultiPlaneCopyReadCmd[0],NandStorageInfo.OptPhyOpPar.MultiPlaneCopyReadCmd[1],
        NandStorageInfo.OptPhyOpPar.MultiPlaneCopyReadCmd[2]);
    SCAN_DBG("[SCAN_DBG]    MultiPlaneCopyWriteCmd: 0x%x, 0x%x, 0x%x\n",
        NandStorageInfo.OptPhyOpPar.MultiPlaneCopyWriteCmd[0], NandStorageInfo.OptPhyOpPar.MultiPlaneCopyWriteCmd[1],
        NandStorageInfo.OptPhyOpPar.MultiPlaneCopyWriteCmd[2]);
    SCAN_DBG("[SCAN_DBG]    MultiPlaneStatusCmd:    0x%x\n", NandStorageInfo.OptPhyOpPar.MultiPlaneStatusCmd);
    SCAN_DBG("[SCAN_DBG]    InterBnk0StatusCmd:     0x%x\n", NandStorageInfo.OptPhyOpPar.InterBnk0StatusCmd);
    SCAN_DBG("[SCAN_DBG]    InterBnk1StatusCmd:     0x%x\n", NandStorageInfo.OptPhyOpPar.InterBnk1StatusCmd);
    SCAN_DBG("[SCAN_DBG]    BadBlockFlagPosition:   0x%x\n", NandStorageInfo.OptPhyOpPar.BadBlockFlagPosition);
    SCAN_DBG("[SCAN_DBG]    MultiPlaneBlockOffset:  0x%x\n", NandStorageInfo.OptPhyOpPar.MultiPlaneBlockOffset);
    SCAN_DBG("[SCAN_DBG] =======================================================\n");

    return 0;
}

