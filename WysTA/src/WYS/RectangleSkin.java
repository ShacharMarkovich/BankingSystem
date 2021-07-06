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

public class RectangleSkin 
{
   protected RectangleSkin()  { }
   
   protected void createRectangleSkin(short borderWidth, int borderColor, int bgColor)
   {
      m_borderWidth = borderWidth;
      m_borderColor = borderColor;
      m_bgColor = bgColor;
   }
   
   public static RectangleSkin create(short borderWidth, int borderColor, int bgColor)
   {
      RectangleSkin rs = new RectangleSkin();
      
      rs.createRectangleSkin(borderWidth, borderColor, bgColor);

      return rs;
   }
   
   public short getBorderWidth() 
   { 
      return m_borderWidth; 
   }
   
   public int getBorderColor()
   {
      return m_borderColor;
   }
   
   public int getBgColor()
   {
      return m_bgColor;
   }
    
   
   protected short m_borderWidth;
   protected int m_borderColor; 
   protected int m_bgColor; 
}
