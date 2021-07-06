/*++
   INTEL CONFIDENTIAL
   Copyright (c) 2010 Intel Corporation. All Rights Reserved.

   The source code contained or described herein and all documents related
   to the source code ("Material") are owned by Intel Corporation or its
   suppliers or licensors. Title to the Material remains with Intel Corporation
   or its suppliers and licensors. The Material contains trade secrets and
   proprietary and confidential information of Intel or its suppliers and
   licensors. The Material is protected by worldwide copyright and trade secret
   laws and treaty provisions. No part of the Material may be used, copied,
   reproduced, modified, published, uploaded, posted, transmitted, distributed, 
   or disclosed in any way without Intel's prior express written permission.

   No license under any patent, copyright, trade secret or other intellectual
   property right is granted to or conferred upon you by disclosure or delivery
   of the Materials, either expressly, by implication, inducement, estoppel or
   otherwise. Any license under such intellectual property rights must be
   express and approved by Intel in writing.
--*/
package WYS;

import com.intel.ui.Label;

public class ButtonSkin extends RectangleSkin 
{


   protected ButtonSkin()  { }
   
   public static ButtonSkin create(short borderWidth, int borderColor, int bgColor, int fontColor, byte fontType)
   {
      ButtonSkin bs = new ButtonSkin();
      
      bs.createRectangleSkin(borderWidth, borderColor, bgColor);
      
      bs.m_fontColor = fontColor;
      bs.m_fontType = fontType;      
      return bs;
   }
   
   public static ButtonSkin create(short borderWidth, int borderColor, int bgColor, int fontColor)
   {
      return create(
                    borderWidth,
                    borderColor,
                    bgColor,
                    fontColor, 
                    getPlatformFontType()
                   );
   }
   
   public byte getFontType()
   {
      return m_fontType;
   }
   
   private static byte getPlatformFontType()
   {
	   //for ME9.5 and below
	   //return Label.FONT_TYPE_NEO_SANS_INTEL;
	   //for ME10 and above
	   return Label.FONT_TYPE_CLEAR_SANS;
   }
    
   protected int m_fontColor;
   protected byte m_fontType;

}
