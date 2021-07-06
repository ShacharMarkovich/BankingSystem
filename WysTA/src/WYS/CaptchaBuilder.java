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
import com.intel.ui.*;

/**
* This class represents a Builder for different <code>CAPTCHA</code> implementations
*/
public class CaptchaBuilder 
{
	
	private CaptchaBuilder(){}


	protected static class Captcha { // Singleton
	
		//private static final int UPPERCASE 		= 0;
		private static final int LOWERCASE 		= 0;
		private static final int DIGIT 			= 1;
		private static final int ALPHA_NUMER 	= 2;

		private String captchaStr;
		private static Captcha me = null;
		
		private Captcha(){
			captchaStr = null;
		};
		
		public static Captcha getInstance(){
			if (me == null){
				me = new Captcha();
			}
			return me;
		}
		
		public void setCaptcha(String definedCapthca){
			captchaStr = definedCapthca;
		}

		public void generateCaptcha(int length)
		{
			if (length <= 0)
			{
				throw new UiIllegalParameterException();
			}
			
			captchaStr = new String("");
			char c = 0;
			byte[] randBytes = new byte[(length << 1)];
			
			Random.getRandomBytes(randBytes, (short)0, (short)randBytes.length);
			for(int i=0; i < length; i++){
				// get absolute values
				if (randBytes[(i<<1)] < 0){
					randBytes[(i<<1)] *= (-1);
				}
				if (randBytes[(i<<1)+1] < 0){
					randBytes[(i<<1)+1] *= (-1);
				}
				
				//determine which type of character to generate
				switch(randBytes[(i<<1)] % ALPHA_NUMER){
				/*case UPPERCASE: //upper case
					c = (char) ((randBytes[(i<<1)+1] % ('Z' - 'A')) + 'A');
					break;*/
				case LOWERCASE: //lower case
					c = (char) ((randBytes[(i<<1)+1] % ('z' - 'a')) + 'a');
					break;
				case DIGIT: // digits
					c = (char) ((randBytes[(i<<1)+1] % ('9' - '0')) + '0');
					break;
				}
				captchaStr += c;			
			}
		}
		
		
		public String 	getString(){return captchaStr;}
		public int 		getStringLenght(){
			if (captchaStr == null){
				return -1;
			}
			return captchaStr.length();
		}

		public boolean equalsTo(String captcha) {
			if (captchaStr == null || captcha == null){
				return false;
			}
			return captchaStr.equals(captcha);
		}
		
	}

	
   /**
    * Creates standard window with random ASCII characters in it.
    * Number of characters is set by final member.
    * The characters appears in the row at the center of the window
    * @param id 			component id
    * @param captchaSize 	The <code>CompositeWidget</code> size in pixels. Should be large enough to contain the captcha
    * 						characters.
    * @param captchaLoc 	The location relative to the parent <code>Widget</code>
    * @param winColor 		The CAPTCHA widget background color.
	* @param fontType		one of Label.FONT_* types
	* @param fontColor 		The CAPTCHA widget foreground color.
	* @param text 			String that contains random generated or pre-defined CAPTCHA characters
    * @return A <code>CompositeWidget</code> instance representing CAPTCHA window
    */
	public static CompositeWidget createCapthaPad(short id, XYPair captchaSize, XYPair captchaLoc, 
												byte frameBorderWidth, int frameColor, int frameBorderColor,
												int fontColor, byte fontType, String text) 
	{
		ButtonSkin buttonSkin = ButtonSkin.create(frameBorderWidth, frameBorderColor, frameColor, fontColor, fontType);
		//padding text with white spaces
		String spaciousText = new String("");
		for(int i = 0; i < text.length(); i++){
			spaciousText += text.charAt(i);
			if(i+1 < text.length()){
				spaciousText += " ";
			}
		}
		CompositeWidget p = ButtonBuilder.createTextButton(id, captchaSize, captchaLoc, spaciousText, buttonSkin);
		return p;
	}

 }
