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

import com.intel.ui.*;

/**
 * This class represents a Builder for different <code>Button</code> implementations
 */
public class ButtonBuilder 
{
   /**
    * Create a <code>Button</code> with text with the specified parameters.
    * The text will be placed in the middle of the <code>Button</code>
    * @param id The ID of the <code>Button</code>
    * @param size The size of the <code>Button</code> 
    * @param relLocation The location relative to the parent <code>Widget</code>
    * @param text The text to appear on the <code>Button</code>
    * @param skin The skin of the button (colors and appearance)
    * @return A <code>Button</code> instance
    */
   public static Button createTextButton(
                                         short id, XYPair size, XYPair relLocation, 
                                         String text, ButtonSkin skin
                                        )
   {
     XYPair widgetLocation = new XYPair((short)0,(short)0);
     
	 size = new XYPair((short)(size.getX() + skin.m_borderWidth*2), (short)(size.getY() + skin.m_borderWidth*2));
      
     Button b = Button.create(id, size, relLocation);	  
	 
	 if ( skin.m_borderWidth > 0 )
	 {
		Rectangle rect = Rectangle.create(id, skin.m_bgColor, size, widgetLocation,
                                       Rectangle.FILL_TYPE_ALL, true, skin.m_borderColor, (short)skin.m_borderWidth);
									   												
		b.addWidget(rect);
	 }
	 else
	 {
		 Rectangle rect = Rectangle.create(id, skin.m_bgColor, size, widgetLocation, 
                                       	Rectangle.FILL_TYPE_ALL, true, 0, (short)0);
		 b.addWidget(rect);
	 }
     XYPair labelSize  = Label.getTextSize(skin.m_fontType, text);
     
     short xMargin =  (short)( ( size.getX() - labelSize.getX() ) / 2);
     short yMargin =  (short)( ( size.getY() - labelSize.getY() ) / 2);
     short xLabelLocation = (short)( widgetLocation.getX() + xMargin );
     short yLabelLocation = (short)( widgetLocation.getY() + yMargin );

     XYPair labelLocation = new XYPair( xLabelLocation, yLabelLocation );
	 Label l = Label.create(
                        	id, skin.m_fontColor, labelLocation, text, 
                        	skin.m_fontType);
     
        
     b.addWidget(l);
     
     return b;
   }
   
   /**
    * Create a <code>Button</code> with an image with the specified parameters
    * The image will be placed in the middle of the <code>Button</code>
    * @param id The ID of the <code>Button</code>
    * @param size The size of the <code>Button</code>
    * @param relLocation The location relative to the parent <code>Widget</code>
    * @param skin The skin of the button (colors and appearance)
    * @param imageSize The image size. Must be smaller than the size of the <code>Button</code>
    * @param imageFormat The image format @see com.intel.ui.Image
    * @param imageData The image data 
    * @return A <code>Button</code> instance
    */
   
   public static Button createImageButton(
                                          short id, XYPair size, XYPair relLocation, 
                                          RectangleSkin skin, 
                                          XYPair imageSize, byte imageFormat, byte[] imageData
                                         )
   {
      /* Check the image size is not larger than the button size */
      if ( imageSize.getX() > size.getX() || imageSize.getY() > size.getY() )
      {
         throw new UiIllegalParameterException();
      }
      
      XYPair rectLocation = new XYPair((short)0,(short)0);
      
      /* Place the image in the center of the button */
      XYPair imageLocation = new XYPair((short)(size.getX()-imageSize.getX()/2),(short)(size.getY()-imageSize.getY()/2));
	  
	  size = new XYPair((short)(size.getX() + skin.m_borderWidth*2), (short)(size.getY() + skin.m_borderWidth*2));
      
      Button b = Button.create(id, size, relLocation);
	  
	  /* Hack - remove when border width works in rendering code */
	 if ( skin.m_borderWidth > 0 )
	 {
		Rectangle border = Rectangle.create(id, skin.m_borderColor, size, rectLocation,
                                       Rectangle.FILL_TYPE_ALL, true, skin.m_borderColor, (short)skin.m_borderWidth);
									   
										
		rectLocation = new XYPair((short)(rectLocation.getX() + skin.m_borderWidth), (short)(rectLocation.getY() + skin.m_borderWidth));
		
		size = new XYPair((short)(size.getX() - skin.m_borderWidth*2), (short)(size.getY() - skin.m_borderWidth*2));
		
		b.addWidget(border);
	 }
      
      Rectangle rect = Rectangle.create(id, skin.m_bgColor, size, rectLocation, Rectangle.FILL_TYPE_ALL, true, 0, (short)0);
      
      Image im = Image.create(id, imageSize, imageLocation, imageFormat, imageData);
      
      b.addWidget(rect);
      b.addWidget(im);
      
      return b;
      
   }
}
