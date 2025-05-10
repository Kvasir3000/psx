#pragma once

#include <stdint.h>

namespace cpu_constants
{
	/* Instruction constants */
	constexpr uint32_t SECONDARY_OPCODE_MASK   = 0x0000003F; /* bits 0....5  */ 
	constexpr uint32_t IMMEDIATE_MASK          = 0x0000FFFF; /* bits 0....16 */
	constexpr uint32_t OFFSET_MASK             = 0x0000FFFF; /* bits 0....16 */
	constexpr uint32_t JUMP_TARGET_MASK        = 0x00FFFFFF; /* bits 0....25 */
	constexpr uint32_t BREAK_CODE_MASK         = 0x03FFFFC0; /* bits 6....25 */
	constexpr uint32_t RD_OPCODE_MASK          = 0x0000F800; /* bits 11...15 */
	constexpr uint32_t RT_OPCODE_MASK          = 0x001F0000; /* bits 16...20 */
	constexpr uint32_t RS_OPCODE_MASK          = 0x03E00000; /* bits 21...25 */
	constexpr uint32_t PRIMARY_OPCODE_MASK     = 0xFC000000; /* bits 26...31 */

	constexpr uint32_t REGIMM_OPCODE           = 0x04000000; /* bit  26      */
	constexpr uint32_t REGIMM_TYPE_MASK        = 0x001F0000; /* bits 16...20 */
	
	constexpr uint32_t COP_OPERATION           = 0x02000000; /* bit  25      */
	constexpr uint32_t COP_SUBOPERATION_MASK   = 0x03E00000; /* bit  21...25 */
	constexpr uint32_t COP_INDEX_MASK          = 0x0C000000; /* bit  26...27 */
	constexpr uint32_t COP_OPCODE              = 0x40000000; /* bit  30      */
	constexpr uint32_t COP_OPCODE_MASK         = 0xF0000000; /* bits 28...31 */

	constexpr uint32_t PC_HIGH_BITS_MASK      =  0xF0000000; /* bits 28...31 */

	constexpr uint32_t RD_OFFSET               = 11;
	constexpr uint32_t RT_OFFSET               = 16;
	constexpr uint32_t REGIMM_TYPE_OFFSET      = 16;
	constexpr uint32_t BREAK_CODE_OFFSET       = 20;
	constexpr uint32_t RS_OFFSET               = 21;
	constexpr uint32_t COP_SUBOPERATION_OFFSET = 21;
	constexpr uint32_t COP_INDEX_OFFSET        = 26;
	constexpr uint32_t PRIMARY_OPCODE_OFFSET   = 26;


	constexpr uint16_t NUMBER_OF_REGISTERS     = 32;
	constexpr uint16_t LINK_REGISTER           = 31; 
	constexpr uint32_t WORD_SIZE               = 4;

	enum DelaySlotState
	{
		None,
		Pending,
		Execute,
	};
}