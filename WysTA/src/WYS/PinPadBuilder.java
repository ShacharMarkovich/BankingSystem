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

import com.intel.crypto.Random;
import com.intel.langutil.TypeConverter;
import com.intel.ui.*;

class Frame extends CompositeWidget
{
   protected Frame() { }
   
   public static Frame createFrame(short id, XYPair size, XYPair relLocation, RectangleSkin skin)
   {
	   Frame p = new Frame();
	  
	  XYPair rectLocation = new XYPair((short)0, (short)0);
	  
	  size = new XYPair((short)(size.getX() + skin.m_borderWidth*2), (short)(size.getY() + skin.m_borderWidth*2));
	  
	  p.createWidget(id, (short)0, size, relLocation);
	  p.m_initialized = true;
      
	  /* Hack - remove when border width works in rendering code */
	 if ( skin.m_borderWidth > 0 )
	 {
		Rectangle border = Rectangle.create(id, skin.m_bgColor, size, rectLocation,
                                       Rectangle.FILL_TYPE_ALL, true, skin.m_borderColor, (short)skin.m_borderWidth);
									   										
		p.addWidget(border);
	 }
	 else
	 {      
		 Rectangle rect = Rectangle.create(
                                       	id, skin.m_bgColor, size, rectLocation, 
                                       	Rectangle.FILL_TYPE_ALL, false, (short)0, (short)0
                                      	);      
		 p.addWidget(rect);
	 }
      
     return p;
   }
}

/**
* This class represents a Builder for different <code>PinPad</code> implementations
*/
public class PinPadBuilder 
{
   public static final short BUTTON_SPACING_HORIZONTAL = 7;
   public static final short BUTTON_SPACING_VERTICAL = 7;
   public static final short MARGIN_SPACING_HORIZONTAL = 10;
   public static final short MARGIN_SPACING_VERTICAL = 10;
   
   public static final short PIN_PAD_BUTTON_COUNT = 10;
   public static final short PIN_PAD_ROW_BUTTON_COUNT = 3;
   public static final short PIN_PAD_COLUMN_BUTTON_COUNT = 4;
   
   public static final short OK_BOX_BUTTON_TO_PAD_SIZE_RATIO = 4;
   
   private static final short MAX_SHORT = 0x7FFF;
   
   
   /**
    * Creates a pin-pad with 10 buttons with the digits from 0-9 in fixed locations.
    * The fixed locations are two rows of 5 buttons.
    * The digits themselves randomly placed among the buttons.
    * Each button has a corresponding id with the digit
    * @param size The <code>CompositeWidget</code> size in pixels. Should be large enough to contain the buttons
    * and enough space for margins and button spacing
    * @param relLocation The location relative to the parent <code>Widget</code>
    * @param buttonSize The size of the buttons in pixels
    * @param dialogBgColorIndex The background color for the dialog
    * @param buttonColorIndex The background color for the buttons
    * @return A <code>CompositeWidget</code> instance representing this pin-pad
    */
   public static CompositeWidget createFixedPinPad(
                                            short id, XYPair size, XYPair relLocation, XYPair buttonSize, 
                                            RectangleSkin pinPadSkin, ButtonSkin buttonSkin 
                                           )
   {
      int xSizeNeeded,  ySizeNeeded;
      int xButtonRowSize, yButtonColumnSize;
      
      /* Check the dialog size is enough to contain all the objects 
       * We have 2 rows of buttons with spacings and margins  
       * The button rows contain 5 objects with spacings and margins 
       */
      
      /* Check Y size */
      yButtonColumnSize = PIN_PAD_COLUMN_BUTTON_COUNT*buttonSize.getY()+(PIN_PAD_COLUMN_BUTTON_COUNT-1)*BUTTON_SPACING_VERTICAL;
      ySizeNeeded = yButtonColumnSize + 2*MARGIN_SPACING_VERTICAL;
      
      if ( (ySizeNeeded > MAX_SHORT) || (ySizeNeeded > size.getY()) )
      {
         throw new UiIllegalParameterException();
      }
      
      /* Check X size */
      xButtonRowSize = PIN_PAD_ROW_BUTTON_COUNT*buttonSize.getX() + (PIN_PAD_ROW_BUTTON_COUNT-1)*BUTTON_SPACING_HORIZONTAL;
      xSizeNeeded = xButtonRowSize + 2*MARGIN_SPACING_HORIZONTAL;
      
      if ( (xSizeNeeded > MAX_SHORT) || (xSizeNeeded > size.getX()) )
      {
         throw new UiIllegalParameterException();
      }
      
      Frame f = Frame.createFrame(id, size, relLocation, pinPadSkin);
      
      /* Now calculate the X,Y placement of button rows
       * We compute the start location so the space left from both sides is equal so the button row will be 
       * placed symmetrically in the middle
       */
      
      short xButtonRowStart = (short)((size.getX() - xButtonRowSize)/2);
      short yButtonColumnStart = (short)((size.getY() - yButtonColumnSize)/2);

      byte[] digits = getShuffledDigits(PIN_PAD_BUTTON_COUNT);
      
      /* Calculate the first row of buttons placement */
      XYPair firstButtonLocation =  new XYPair(xButtonRowStart,yButtonColumnStart);
      
      XYPair curButtonLocation;
      
      //DebugPrint.printString("ADDING BUTTONS");
      /* Now place the button rows */
      for ( int i = 0; i < PIN_PAD_COLUMN_BUTTON_COUNT; ++i )
      {
         curButtonLocation = firstButtonLocation;
         
         for ( int j = i*PIN_PAD_ROW_BUTTON_COUNT; j < (i+1)*PIN_PAD_ROW_BUTTON_COUNT; ++j )
         {
        	if (j == (PIN_PAD_BUTTON_COUNT-1) )
        	{
        		// 1 Last button draw in the center
        		curButtonLocation = new XYPair((short)(curButtonLocation.getX() + buttonSize.getX() + BUTTON_SPACING_HORIZONTAL),curButtonLocation.getY());        	
        	}
        	
        	if ( j < PIN_PAD_BUTTON_COUNT )
        	{
        		Button b = ButtonBuilder.createTextButton((short)(digits[j]), buttonSize, curButtonLocation, ""+digits[j], buttonSkin); 
            
        		f.addWidget(b);
        		
        		/* Update the new button horizontal location */
        		curButtonLocation = new XYPair((short)(curButtonLocation.getX() + buttonSize.getX() + BUTTON_SPACING_HORIZONTAL),curButtonLocation.getY());
        	}
         }
         
         /* Update the next row vertical location */
         firstButtonLocation =  new XYPair(firstButtonLocation.getX(), (short)(firstButtonLocation.getY() + buttonSize.getY() +  BUTTON_SPACING_VERTICAL));
      }
      //DebugPrint.printString("DONE");
       
      return f;
   }

   
   /**
    * Creates standard window with OK button in it.
    * The button locations is randomly generated.
    * @param size The <code>CompositeWidget</code> size in pixels. Should be large enough to contain the button
    * at random location.
    * @param relLocation The location relative to the parent <code>Widget</code>
    * @param buttonSize The size of the button in pixels
    * @param dialogBgColorIndex The background color for the dialog
    * @param buttonColorIndex The background color for the buttons
    * @param buttonText	Text on button component.
    * @return A <code>CompositeWidget</code> instance representing this ok-button pad
    */
   public static CompositeWidget createOkBox(short id, XYPair size, XYPair relLocation, XYPair buttonSize, RectangleSkin pinPadSkin, ButtonSkin buttonSkin, String buttonText) 
   {
      byte[] randNums = new byte[TypeConverter.SHORT_BYTE_SIZE << 1];
      short x,y;
      
      /* Check Y size */
      int ySizeNeeded =  OK_BOX_BUTTON_TO_PAD_SIZE_RATIO * buttonSize.getY() + 2 * MARGIN_SPACING_VERTICAL;
      if ( (ySizeNeeded > MAX_SHORT) || (ySizeNeeded > size.getY()) )
      {
         throw new UiIllegalParameterException();
      }
      
      /* Check X size */
      int xSizeNeeded =  OK_BOX_BUTTON_TO_PAD_SIZE_RATIO * buttonSize.getX() + 2 * MARGIN_SPACING_HORIZONTAL;
      if ( (xSizeNeeded > MAX_SHORT) || (xSizeNeeded > size.getX()) )
      {
         throw new UiIllegalParameterException();
      }
      
      Frame f = Frame.createFrame(id, size, relLocation, pinPadSkin);

      //get random bytes
      Random.getRandomBytes(randNums, (short)0, (short)randNums.length);
      x = TypeConverter.bytesToShort(randNums, 0);
      y = TypeConverter.bytesToShort(randNums, TypeConverter.SHORT_BYTE_SIZE);
      
      //absolute values
      if (x < 0)
      {
    	  x = (short) (x * (-1));
      }
      if (y < 0)
      {
    	  y = (short) (y * (-1));
      }
      
      //modulo to fit CompositeWidget size
      x = (short) ((x % (size.getX()-buttonSize.getX()-(pinPadSkin.getBorderWidth()<<1))) + pinPadSkin.getBorderWidth());
      y = (short) ((y % (size.getY()-buttonSize.getY()-(pinPadSkin.getBorderWidth()<<1))) + pinPadSkin.getBorderWidth());
      
      //create button at random location
      XYPair buttonLoc = new XYPair((short)(x), (short)(y));
	  Button b = ButtonBuilder.createTextButton((short)0, buttonSize, buttonLoc, buttonText, buttonSkin);
	  //add button to widget
	  f.addWidget(b);
       
      return f;
   }
   
   /**
    * Creates a pin-pad with 10 buttons in random locations.
    * The digits are also randomly placed among the buttons.
    * Each button has a corresponding id with the digits
    * @param size The <code>CompositeWidget</code> size in pixels. Should be large enough to contain the buttons
    * and enough space for margins and button spacing
    * @param relLocation The location relative to the parent <code>Widget</code>
    * @param buttonSize The size of the buttons in pixels
    * @param dialogBgColorIndex The background color for the dialog
    * @param buttonColorIndex The background color for the buttons
    * @return A <code>CompositeWidget</code> instance representing this pin-pad
    */
   public static CompositeWidget createRandomPinPad(
                                                  short id, XYPair size, XYPair relLocation, XYPair buttonSize, 
                                                  RectangleSkin pinPadSkin, ButtonSkin buttonSkin 
                                                 )
   {
      
      /* Divide the PinPad to equal cells in the size of the buttons + padding */
      
      int xSizeNoMargin = size.getX() - MARGIN_SPACING_HORIZONTAL*2;
      int ySizeNoMargin = size.getY() - MARGIN_SPACING_VERTICAL*2;
      int xPaddedButtonSize = buttonSize.getX() + BUTTON_SPACING_HORIZONTAL;
      int yPaddedButtonSize = buttonSize.getY() + BUTTON_SPACING_VERTICAL;
      
      int xNumCells = xSizeNoMargin/xPaddedButtonSize;
      int yNumCells = ySizeNoMargin/yPaddedButtonSize;
	  
	  
	  //DebugPrint.printString("Got numCells = "+xNumCells+","+yNumCells);

      /* Check that we have enough space for the buttons */
      /* TODO: we actually need more than just the amount for buttons - maybe a factor of 2 */
      if ( xNumCells*yNumCells < PIN_PAD_BUTTON_COUNT )
      {
         throw new UiIllegalParameterException();
      }
      
      Frame f = Frame.createFrame(id, size, relLocation, pinPadSkin);

      /* Create a shuffled array the cell indices */
      byte[] cells = getShuffledDigits(xNumCells*yNumCells);
      
      /* Create an array of the shuffled button digits */
      byte[] digits = getShuffledDigits(PIN_PAD_BUTTON_COUNT);
	  
      
      /* Place the button[digit[i]] in cell[i] */
      for ( int i = 0; i < PIN_PAD_BUTTON_COUNT; ++i )
      {
   	    //DebugPrint.printString("getting place for random button "+i);
         
         /* We extract the x and y cell indices */
   		int xCell = cells[i]%xNumCells;
   		int yCell = cells[i]/yNumCells;
		
   		//DebugPrint.printString("cell ="+cells[i]+" xCell ="+xCell+" yCell ="+yCell);
		
         /* Calculate the placement of the button according to the cell number */
         short xButtonLoc = (short)(MARGIN_SPACING_HORIZONTAL + xCell*buttonSize.getX() + (xCell*BUTTON_SPACING_HORIZONTAL) + (BUTTON_SPACING_HORIZONTAL/2)); 
         short yButtonLoc = (short)(MARGIN_SPACING_VERTICAL + yCell*buttonSize.getY()  + (yCell*BUTTON_SPACING_VERTICAL) + (BUTTON_SPACING_VERTICAL/2));
         
         XYPair buttonLoc = new XYPair(xButtonLoc,yButtonLoc);
         
         Button b = ButtonBuilder.createTextButton((short)(digits[i]), buttonSize, 
              buttonLoc, ""+digits[i], buttonSkin);
         
         f.addWidget(b);
      }
      
      return f;
   }
   
  
	private static byte[] getShuffledDigits(int numDigits)
	{
		byte[] digits = new byte[numDigits];
		byte[] random = new byte[numDigits*TypeConverter.INT_BYTE_SIZE];
		byte[] position=new byte[numDigits];      
		// Generate the digit array (0...n-1  ordered digits)
		for ( int i = 0; i < digits.length; ++i )
		{
		  digits[i] = (byte)i;
		}
		//Generate random array
		Random.getRandomBytes(random, (short)0, (short)random.length);	    
		long absRandom;
		//Shuffle the digits 
		for (int i=0; i<numDigits; i++)
		{	 
			 //Convert 4 bytes to integer  
			 int temp=TypeConverter.bytesToInt(random, i*TypeConverter.INT_BYTE_SIZE);
			 //Get the absolute number of the integer
			 absRandom = (temp > 0) ? (temp) : (long)(-temp);
			 //Module the number to an index of 0 to n-i
			 int x =(int) (absRandom % (numDigits-i));
			 //Save the randomly selected number to the result array
			 position[i] = digits[x];  
			 //Put the value from the end of the remaining list into the newly vacant space.
			 digits[x] = digits[numDigits-i-1];  
		}
		return position;
	}

}
