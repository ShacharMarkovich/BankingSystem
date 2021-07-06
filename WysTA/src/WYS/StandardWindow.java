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

import WYS.CaptchaBuilder.Captcha;

import com.intel.crypto.Random;
import com.intel.langutil.ArrayUtils;
import com.intel.langutil.TypeConverter;
import com.intel.ui.Dialog;
import com.intel.ui.Image;
import com.intel.ui.Label;
import com.intel.ui.ProtectedOutput;
import com.intel.ui.UiException;
import com.intel.ui.UiIllegalParameterException;
import com.intel.ui.Widget;
import com.intel.ui.WidgetMapping;
import com.intel.ui.XYPair;
import com.intel.util.DebugPrint;
import com.intel.util.IntelApplet;

/**
 * This utility class provides applet with macro tools for creation, display and input processing
 * for pre-defined ProtectedOutput dialog types: PIN pad, CAPTCHA and OK box.
 * Additional functionality exposed by this class is ProtectedOutput context access for remote
 * windows rendering.
 * This class provides message processing function to be invoked when standard windows messages are received
 * from software and also API for retrieval results of user input. Applet has to obtain instance of the class
 * and use it for message processing and result retrieval.
 */
public class StandardWindow 
{
	public final static int STANDARD_COMMAND_ID = 0xFFFF0001;
	
	private final static byte WINDOW_TYPE_UNKNOWN	= -1;
	private final static byte WINDOW_TYPE_PINPAD	= 1;
	private final static byte WINDOW_TYPE_OK		= 2;
	private final static byte WINDOW_TYPE_CAPTCHA	= 3;
	
	private final static byte SUB_COMMAND_BUILD_WINDOW		= 1;
	private final static byte SUB_COMMAND_SET_KEY			= 2;	
	private final static byte SUB_COMMAND_GET_IMAGE_CHUNK	= 3;
	private final static byte SUB_COMMAND_SUBMIT_INPUT		= 4;
	private final static byte SUB_COMMAND_CANCEL			= 5;

	// subCommand + windowType + slot + appParam + winSize + frameBorderWidth + 
	// buttonSize + buttonBorderWidth + fontType + textLength + colors + logoSize
	private final static int BUILD_WINDOW_REQUEST_MIN_SIZE = 
												2 + 2 * TypeConverter.INT_BYTE_SIZE + 2 * TypeConverter.SHORT_BYTE_SIZE +
												1 + 2 * TypeConverter.SHORT_BYTE_SIZE + 3 + 6 * TypeConverter.INT_BYTE_SIZE +
												2 * TypeConverter.SHORT_BYTE_SIZE ;
	
	// subCommand + slot
	private final static int SET_KEY_REQUEST_MIN_SIZE = 2 + TypeConverter.SHORT_BYTE_SIZE;
												
	// subCommand + handle + size
	private final static int GET_IMAGE_CHUNK_REQUEST_MIN_SIZE = 1 + 2 * TypeConverter.INT_BYTE_SIZE;
	
	// subCommand + clicksCount OR subCommand + textSize
	private final static int SUBMIT_INPUT_REQUEST_MIN_SIZE = 2;
	
	// subCommand
	private final static int CANCEL_REQUEST_MIN_SIZE = 1;
	

	private final static byte WIDGET_ENTRY_SIZE = (TypeConverter.SHORT_BYTE_SIZE * 4); 
	private final static byte PINPAD_MAP_SIZE 	= (WIDGET_ENTRY_SIZE * 10);
	
	private final static int INVALID_APP_PARAM = -1;
	
	private final static String OK_BOX_TEXT = "OK";
	
	private final static int INTEL_LOGO_24BPP_PIXEL_SIZE = 3;
	
	private ProtectedOutput m_protectedOutput;
	private Dialog 			m_dialog;
	private int 			m_imageSize;
	private byte 			m_windowType;
	private int 			m_imageHandle;
	private Captcha 		m_captchaText;
	private boolean 		m_userAuthenticated;
	private byte[] 			m_userPin; 
	private int 			m_applicationParam;
	private byte[] 			m_responseData;
	private int				m_responseLength;
	
	private static final short WIDGET_ID_PINPAD = 4;
	private static final short WIDGET_ID_LOGO = 5;
	private static final short WIDGET_ID_OKBUTTON = 6;
	private static final short WIDGET_ID_CAPTCHA = 7;

	/*
	protected static final int COLOR_RGB_WHITE = 0x00FFFFFF;
	protected static final int COLOR_RGB_RED = 0x00FF0000;
	protected static final int COLOR_RGB_GREEN = 0x0000FF00;
	protected static final int COLOR_RGB_BLUE = 0x000000FF;
	protected static final int COLOR_RGB_LIGHT_PURPLE = 0x00DCD0FF;
	*/
	
	private static final short LOGO_MARGIN_X = 10;
	private static final short LOGO_MARGIN_Y = 10;
	
	private static final short STANDARD_FRAME_MARGIN_X = 30;
	private static final short STANDARD_FRAME_MARGIN_Y = 30;
	private byte[] m_sessionKey = new byte [ProtectedOutput.KEY_LENGTH];
	private StandardWindow() {}
	
	/**
	 * This method should be invoked by applet to get a single instance of StandardWindow class.
	 * @return Instance of StandardWindow class.
	 */
	public static final StandardWindow getInstance()
	{
		return new StandardWindow();
	}
	
	private void resetContext()
	{
		m_userAuthenticated = false;
		m_userPin = null;
		m_applicationParam = INVALID_APP_PARAM;
		m_dialog = null;
		m_imageSize = 0;
		m_windowType = WINDOW_TYPE_UNKNOWN;
		m_captchaText = null;
		if ( m_protectedOutput != null )
		{
			try
			{
				m_protectedOutput.releaseImage();
			}
			catch(UiException e){}
		}
		m_protectedOutput = null;
	}
	
	private void initContext(int slotHandle, int appParam)
	{
		resetContext();
		
		byte[] imageHandle = new byte[TypeConverter.INT_BYTE_SIZE];
		Random.getRandomBytes(imageHandle, (short)0, (short)imageHandle.length);
		m_imageHandle = TypeConverter.bytesToInt(imageHandle, 0);
			   
		//The S1 key must be secured, i.e. The applet can generates it randomly or the host can pass it in a  secure way.
		Random.getRandomBytes(m_sessionKey, (short)0, (short)m_sessionKey.length);
		m_protectedOutput = ProtectedOutput.getInstance(slotHandle, m_sessionKey, (short)0, (short)m_sessionKey.length);
		
		m_applicationParam = appParam;
	}

	private static int widgetMapToBytes(WidgetMapping[] map, byte[] response, int offset)
	{
		int progress  = offset;
		
		for (int i = 0; i < map.length; i++)
		{
			TypeConverter.shortToBytes(map[i].getLocation().getX(), response, progress); 
			progress += TypeConverter.SHORT_BYTE_SIZE;
			
			TypeConverter.shortToBytes(map[i].getLocation().getY(), response, progress); 
			progress += TypeConverter.SHORT_BYTE_SIZE;
			
			TypeConverter.shortToBytes(map[i].getSize().getX(), response, progress); 
			progress += TypeConverter.SHORT_BYTE_SIZE;
			
			TypeConverter.shortToBytes(map[i].getSize().getY(), response, progress); 
			progress += TypeConverter.SHORT_BYTE_SIZE;
		}
		
		return (progress - offset);
	}
	
	private byte[] prepareBuildWindowResponse()
	{
		// sub command ID + window type + input handle + encrypted session key + crypto counter length + resulting image size
		int responseSize = 2 + TypeConverter.INT_BYTE_SIZE * 2 + ProtectedOutput.KEY_LENGTH + ProtectedOutput.AES_CTR_IV_LENGTH;
		if ( m_windowType == WINDOW_TYPE_PINPAD )
		{
			// in case of PIN pad we also return the buttons map
			responseSize += PINPAD_MAP_SIZE;
		}
		byte[] response = new byte[responseSize];
		
		response[0] = SUB_COMMAND_BUILD_WINDOW;
		response[1] = m_windowType;
		
		int offset = 2;
		
		TypeConverter.intToBytes(m_imageHandle, response, offset);
		offset += TypeConverter.INT_BYTE_SIZE;
		
		offset += m_protectedOutput.getEncryptedKeyRecord(response, (short)offset);
		
		offset += m_protectedOutput.getIV(response, (short)offset);
		
		TypeConverter.intToBytes(m_imageSize, response, offset);
		offset += TypeConverter.INT_BYTE_SIZE;
		
		if ( m_windowType == WINDOW_TYPE_PINPAD )
		{
			offset += widgetMapToBytes(m_dialog.getClickableWidgetMappings(), response, offset);
		}
		
		return response;
	}
	
	private int processBuildWindow(byte[] request, int offset)
	{
		byte windowType = request[offset];
		offset++;
		
		int slot = TypeConverter.bytesToInt(request, offset);
		offset += TypeConverter.INT_BYTE_SIZE;
		
		int appParam = TypeConverter.bytesToInt(request, offset);
		offset += TypeConverter.INT_BYTE_SIZE;
		
		try
		{
			initContext(slot, appParam);
		}
		catch (UiException ex)
		{
			DebugPrint.printString("Failed to initialize context");
			return IntelApplet.APPLET_ERROR_BAD_PARAMETERS;
		}
		
		short dialogSizeX = TypeConverter.bytesToShort(request, offset);
		offset += TypeConverter.SHORT_BYTE_SIZE;
		
		short dialogSizeY = TypeConverter.bytesToShort(request, offset);
		offset += TypeConverter.SHORT_BYTE_SIZE;
		
		byte frameBorderWidth = request[offset];
		offset++;
		
		short buttonSizeX = TypeConverter.bytesToShort(request, offset);
		offset += TypeConverter.SHORT_BYTE_SIZE;
		
		short buttonSizeY = TypeConverter.bytesToShort(request, offset);
		offset += TypeConverter.SHORT_BYTE_SIZE;
		
		byte buttonBorderWidth = request[offset];
		offset++;
		
		byte fontType = request[offset];
		offset++;
		
		byte textLength = request[offset];
		offset++;
		
		int dialogColor = TypeConverter.bytesToInt(request, offset);
		offset += TypeConverter.INT_BYTE_SIZE;
		
		int frameColor = TypeConverter.bytesToInt(request, offset);
		offset += TypeConverter.INT_BYTE_SIZE;
		
		int frameBorderColor = TypeConverter.bytesToInt(request, offset);
		offset += TypeConverter.INT_BYTE_SIZE;
		
		int buttonColor = TypeConverter.bytesToInt(request, offset);
		offset += TypeConverter.INT_BYTE_SIZE;
		
		int buttonBorderColor = TypeConverter.bytesToInt(request, offset);
		offset += TypeConverter.INT_BYTE_SIZE;
		
		int fontColor = TypeConverter.bytesToInt(request, offset);
		offset += TypeConverter.INT_BYTE_SIZE;
		
		short logoSizeX = TypeConverter.bytesToShort(request, offset);
		offset += TypeConverter.SHORT_BYTE_SIZE;
		
		short logoSizeY = TypeConverter.bytesToShort(request, offset);
		offset += TypeConverter.SHORT_BYTE_SIZE;

		switch (windowType)
		{
			case WINDOW_TYPE_PINPAD:
				
				m_windowType = WINDOW_TYPE_PINPAD;
				
				try
				{
					DebugPrint.printString("####  PinPad  #####");
					m_dialog = createPinPad(new XYPair(dialogSizeX, dialogSizeY),
											frameBorderWidth,
											new XYPair(buttonSizeX, buttonSizeY),
											buttonBorderWidth,
											dialogColor, 
											frameColor,
											frameBorderColor,
											buttonColor, 
											buttonBorderColor,
											fontColor,
											new XYPair(logoSizeX, logoSizeY),
											request,
											offset);
				}
				catch (UiException ex)
				{
					DebugPrint.printString("Failed to build standard PIN pad window");
					return IntelApplet.APPLET_ERROR_BAD_PARAMETERS;
				}
				
				break;
				
			case WINDOW_TYPE_OK:

				m_windowType = WINDOW_TYPE_OK;
				
				try
				{
					DebugPrint.printString("####  OK button  #####");
					m_dialog = createOKBox(new XYPair(dialogSizeX, dialogSizeY),
											frameBorderWidth,
											new XYPair(buttonSizeX, buttonSizeY),
											buttonBorderWidth,
											dialogColor, 
											frameColor,
											frameBorderColor,
											buttonColor, 
											buttonBorderColor,
											fontColor,
											new XYPair(logoSizeX, logoSizeY),
											request,
											offset);
				}
				catch (UiException ex)
				{
					DebugPrint.printString("Failed to build standard OK box window");
					return IntelApplet.APPLET_ERROR_BAD_PARAMETERS;
				}
				
				break;
				
			case WINDOW_TYPE_CAPTCHA:
				
				m_windowType = WINDOW_TYPE_CAPTCHA;

				try
				{
					DebugPrint.printString("####  Captcha  #####");
					m_captchaText = Captcha.getInstance();
					m_captchaText.generateCaptcha(textLength);
					
					m_dialog = createCAPTCHA(new XYPair(dialogSizeX, dialogSizeY),
											frameBorderWidth,
											//for ME9.5 and below
										    //Label.FONT_TYPE_NEO_SANS_INTEL,
											//for ME10 and above
											Label.FONT_TYPE_CLEAR_SANS,
											dialogColor,
											frameColor,
											frameBorderColor,
											fontColor,
											new XYPair(logoSizeX, logoSizeY),
											request,
											offset,
											m_captchaText.getString());
				}
				catch (UiException ex)
				{
					DebugPrint.printString("Failed to build standard CAPTCHA window");
					return IntelApplet.APPLET_ERROR_BAD_PARAMETERS;
				}
				
				break;
				
			default:
				DebugPrint.printString("Wrong standard window type received");
				return IntelApplet.APPLET_ERROR_BAD_PARAMETERS;
		}
		
		try
		{
			m_imageSize = m_protectedOutput.startRendering(m_dialog, ProtectedOutput.RENDERING_FORMAT_XRGB);
		}
		catch (UiException ex)
		{
			DebugPrint.printString("Failed to render the standard window");
			return IntelApplet.APPLET_ERROR_BAD_PARAMETERS;
		}
		
		m_responseData = prepareBuildWindowResponse();
		m_responseLength = m_responseData.length;
		
		return IntelApplet.APPLET_SUCCESS;
	}
	
	private int processSetKey(byte[] request, int offset)
	{
		short slot = TypeConverter.bytesToShort(request, offset);
		
		try
		{
			initContext(slot, INVALID_APP_PARAM);
		}
		catch (UiException ex)
		{
			resetContext();
			DebugPrint.printString("Failed to initialize context");
			return IntelApplet.APPLET_ERROR_BAD_PARAMETERS;
		}
		
		m_responseLength = ProtectedOutput.KEY_LENGTH + 1;
		m_responseData = new byte[m_responseLength];
		
		m_responseData[0] = SUB_COMMAND_SET_KEY;
		m_protectedOutput.getEncryptedKeyRecord(m_responseData, (short)1);
		
		return IntelApplet.APPLET_SUCCESS;
	}
	
	private int processGetImageChunk(byte[] request, int offset)
	{
		int imageHandle = TypeConverter.bytesToInt(request, offset);
		if ( imageHandle != m_imageHandle )
		{
			return IntelApplet.APPLET_ERROR_BAD_PARAMETERS;
		}
		offset += TypeConverter.INT_BYTE_SIZE;

		int chunkSize = TypeConverter.bytesToInt(request, offset);
		offset += TypeConverter.INT_BYTE_SIZE;
		
		m_responseData = new byte[chunkSize];
		
		try
		{
			m_responseLength = m_protectedOutput.getImageBlock(m_responseData, 0, chunkSize);
		}
		catch (UiException e)
		{
			DebugPrint.printString("Failed to get image block of size: " + chunkSize);
			return IntelApplet.APPLET_ERROR_BAD_STATE;
		}
		
		return IntelApplet.APPLET_SUCCESS;
	}
	
	private XYPair[] bytesToClicks(byte[] request, int offset)
	{
		if ( request[offset] <= 0 )
		{
			// no clicks, return empty array
			return new XYPair[0];
		}
		
		XYPair[] clicks = new XYPair[request[offset]];
		offset++;
		
		if ( (request.length - offset) < (clicks.length * 2 * TypeConverter.SHORT_BYTE_SIZE) )
		{
			// wrong request size, return empty array
			return new XYPair[0];
		}

		short xClick, yClick;
		
		for (byte i = 0; i < clicks.length; i++)
		{
			xClick = TypeConverter.bytesToShort(request, offset);
			offset += TypeConverter.SHORT_BYTE_SIZE;
			
			yClick = TypeConverter.bytesToShort(request, offset);
			offset += TypeConverter.SHORT_BYTE_SIZE;
			
			clicks[i] = new XYPair(xClick, yClick);
		}
		
		return clicks;
	}
	
	private int processSubmitInput(byte[] request, int offset)
	{
		switch ( m_windowType )
		{
			case WINDOW_TYPE_PINPAD:
				
				int i;
				Widget w;
				
				XYPair[] clicks = bytesToClicks(request, offset);
				if ( clicks.length == 0 )
				{
					DebugPrint.printString("Failed to parse submitted clicks");
					return IntelApplet.APPLET_ERROR_BAD_PARAMETERS;
				}
				m_userPin = new byte[clicks.length];
				
				for (i = 0; i < clicks.length; i++)
				{
					w = m_dialog.getClickedWidget(clicks[i]);
					if ( w == null )
					{
						m_userPin = null;
						DebugPrint.printString("One of the clicks is outside buttons");
						return IntelApplet.APPLET_ERROR_BAD_PARAMETERS;
					}

					m_userPin[i] = (byte)w.getId();
				}
				
				break;
				
			case WINDOW_TYPE_OK:
				
				clicks = bytesToClicks(request, offset);
				if ( clicks.length != 1 )
				{
					DebugPrint.printString("Failed to parse submitted click");
					return IntelApplet.APPLET_ERROR_BAD_PARAMETERS;
				}
				
				w = m_dialog.getClickedWidget(clicks[0]);
				if ( w != null )
				{
					m_userAuthenticated = true;
				}
				else
				{
					m_userAuthenticated = false;
				}
				
				break;
				
			case WINDOW_TYPE_CAPTCHA:
				
				String inputText = new String("");
				byte inputTextLength = request[offset];
				offset++;
				
				if ( (request.length - offset) < inputTextLength )
				{
					DebugPrint.printString("Failed to parse submitted text");
					return IntelApplet.APPLET_ERROR_BAD_PARAMETERS;
				}
				
				for (i = 0; i < inputTextLength; i++)
				{
					inputText += (char)(request[offset + i]);
				}
				
				if ( m_captchaText.equalsTo(inputText) )
				{
					m_userAuthenticated = true;
				}
				else
				{
					m_userAuthenticated = false;
				}

				break;
				
			default:
				
				DebugPrint.printString("Failed to submit input, since no window was rendered");
				return IntelApplet.APPLET_ERROR_BAD_STATE;
		}
		
		return IntelApplet.APPLET_SUCCESS;
	}
	
	private int processCancel(byte[] request, int offset)
	{
		resetContext();
		
		return IntelApplet.APPLET_SUCCESS;
	}

	private int verifyRequestSize(byte[] request, int requestOffset)
	{
		int minRequestSize;
		int requestLength = request.length - requestOffset;
		
		if ( requestLength < 1 )
		{
			// at least the sub command must be included in the request
			return IntelApplet.APPLET_ERROR_BAD_PARAMETERS;
		}
		
		switch ( request[requestOffset] )
		{
			case SUB_COMMAND_BUILD_WINDOW:
				minRequestSize = BUILD_WINDOW_REQUEST_MIN_SIZE;
				break;
				
			case SUB_COMMAND_SET_KEY:
				minRequestSize = SET_KEY_REQUEST_MIN_SIZE;
				break;
				
			case SUB_COMMAND_GET_IMAGE_CHUNK:
				minRequestSize = GET_IMAGE_CHUNK_REQUEST_MIN_SIZE;
				break;
				
			case SUB_COMMAND_SUBMIT_INPUT:
				minRequestSize = SUBMIT_INPUT_REQUEST_MIN_SIZE;
				break;
				
			case SUB_COMMAND_CANCEL:
				minRequestSize = CANCEL_REQUEST_MIN_SIZE;
				break;
				
			default:
				return IntelApplet.APPLET_ERROR_BAD_PARAMETERS;					
		}
		
		if ( requestLength < minRequestSize )
		{
			return IntelApplet.APPLET_ERROR_BAD_PARAMETERS;
		}
		
		return IntelApplet.APPLET_SUCCESS;
	}
	
	/**
	 * This method should be invoked from applet's invokeCommand method if the command ID
	 * received from application side equals to StandardWindow.STANDARD_COMMAND_ID in order 
	 * to process the command data. <code>getResponse</code> method should be used after calling
	 * this method in order to fill in the response data to be returned to the host side application. 
	 * @param commandId the command ID as received by applet in invokeCommand
	 * @param request request data as received by the applet in invokeCommand 
	 * @param requestOffset offset in the request data where the standard command data begins (to
	 * 						allow applet to add application specific headers to the message)
	 * @return status code to be returned by the applet from invokeCommand
	 */
	public int processCommand(int commandId, byte[] request, int requestOffset)
	{
		int retCode;
		
		if ( commandId != STANDARD_COMMAND_ID ) 
		{
			DebugPrint.printString("Non standard command received");
			return IntelApplet.APPLET_ERROR_BAD_PARAMETERS;
		}
		
		m_responseData = null;
		m_responseLength = 0;
		
		retCode = verifyRequestSize(request, requestOffset);
		if ( retCode != IntelApplet.APPLET_SUCCESS )
		{
			DebugPrint.printString("Wrong standard request size");
			return retCode;
		}

		switch ( request[requestOffset] ) // first byte is sub command ID
		{
			case SUB_COMMAND_BUILD_WINDOW:
				retCode = processBuildWindow(request, requestOffset + 1);
				if ( retCode != IntelApplet.APPLET_SUCCESS )
				{
					resetContext();
				}
				break;
				
			case SUB_COMMAND_SET_KEY:
				retCode = processSetKey(request, requestOffset + 1);
				if ( retCode != IntelApplet.APPLET_SUCCESS )
				{
					resetContext();
				}
				break;
				
			case SUB_COMMAND_GET_IMAGE_CHUNK:
				retCode = processGetImageChunk(request, requestOffset + 1);
				break;
				
			case SUB_COMMAND_SUBMIT_INPUT:
				retCode = processSubmitInput(request, requestOffset + 1);
				break;
				
			case SUB_COMMAND_CANCEL:
				retCode = processCancel(request, requestOffset + 1);
				break;
				
			default:
				DebugPrint.printString("Wrong sub command ID received");
				retCode = IntelApplet.APPLET_ERROR_BAD_PARAMETERS;
				break;
		}
		
		if ( retCode != IntelApplet.APPLET_SUCCESS )
		{
			m_responseData = null;
			m_responseLength = 0;
		}
			
		return retCode;
	}
	
	/**
	 * This method should be used by the applet to retrieve the response data to be sent
	 * back to the host side application after calling the <code>processCommand</code> method.  
	 * @param response the destination array
	 * @param responseOffset offset in the destination array
	 * @return the number of bytes copied into the destination array
	 */
	public int getResponse(byte[] response, int responseOffset)
	{
		if ( m_responseData != null )
		{
			ArrayUtils.copyByteArray(m_responseData, 0, response, responseOffset, m_responseLength);
			return m_responseLength;
		}
		else
		{
			return 0;
		}
	}

	/**
	 * This method can be used by the applet to retrieve the size of the response data 
	 * to be returned to the host side application after calling the <code>processCommand</code>
	 * method.
	 * @return the number of bytes to be returned to the host side application after processing a command.
	 */
	public int getResponseSize()
	{
		return m_responseLength;
	}
	
	/**
	 * This method is invoked by applet to retrieve user entered PIN code.
	 * Applet has to receive trigger from host application informing that user's clicks
	 * have been forwarded to the StandardWindow handler and may now be retrieved.
	 * @return Byte array containing digits in the range 0..9, single digit per byte.
	 */
	public byte [] getPin()
	{
		return m_userPin;
	}
	
	/**
	 * This method is invoked to retrieve status of user input. 
	 * In case of CAPTCHA window, return value is true if user's input matches CAPTCHA string, 
	 * and false otherwise. 
	 * In case of OK window, return value is true if user's click hit the button, and false in case of miss.
	 * Applet has to receive trigger from host application informing that user's clicks
	 * have been forwarded to the StandardWindow handler and may now be retrieved.
	 * @return User input status.
	 */
	
	public boolean getUserInputStatus()
	{
		return m_userAuthenticated;
	}
	
	/**
	 * This method is invoked to get access to the ProtectedOutput context to be used for server
	 * rendered windows. The API may be invoked after host side application provides StandardWindow
	 * infrastructure with ProtectedOutput slot ID. 
	 * Applet has to receive trigger from host application informing that slot ID
	 * have been forwarded to the StandardWindow handler and may now be retrieved.
	 * @return Instance of the initialized ProtectedOutput class.
	 */
	public ProtectedOutput getSlotContext()
	{
		return m_protectedOutput;
	}
	
	/**
	 * This method returns the application parameter that was sent by the SW application
	 * during the creation of one of the standard windows.
	 * @return The application parameter defined during window creation.
	 */
	public int getApplicationParam()
	{
		return m_applicationParam;
	}
	
	private static XYPair getStandardFrameSize(XYPair winSize, XYPair logoSize)
	{
		int frameSizeX = winSize.getX() - 2 * STANDARD_FRAME_MARGIN_X;
		int frameSizeY = winSize.getY() - 2 * STANDARD_FRAME_MARGIN_Y;
	
		if ( (logoSize.getX()) > 0 && (logoSize.getY() > 0) )
		{
			frameSizeX = frameSizeX - logoSize.getX() - 2 * LOGO_MARGIN_X + STANDARD_FRAME_MARGIN_X;
			frameSizeY = frameSizeY - logoSize.getY() - 2 * LOGO_MARGIN_Y + STANDARD_FRAME_MARGIN_Y;
		}

		return new XYPair((short)frameSizeX, (short)frameSizeY);
	}
	
	private static XYPair getStandardFrameLocation(XYPair winSize, XYPair logoSize)
	{
		int frameLocX;
		int frameLocY;
		
		if ( (logoSize.getX() > 0) && (logoSize.getY() > 0) )
		{
			frameLocX = logoSize.getX() + 2 * LOGO_MARGIN_X;
			frameLocY = logoSize.getY() + 2 * LOGO_MARGIN_Y;
		}
		else
		{
			frameLocX = STANDARD_FRAME_MARGIN_X;
			frameLocY = STANDARD_FRAME_MARGIN_Y;	
		}
		
		return new XYPair((short)frameLocX, (short)frameLocY);
	}
		
	/**
	 * Creates standard PIN pad window.
	 * @param winSize the size of the window
	 * @param buttonSize the size of the buttons
	 * @param winColor window background color
	 * @param buttonColor buttons color
	 * @param logoSize size of the customized logo image
	 * @param logoImage customized logo image data
	 * @param logoIndex start index in the logo data array
	 * @return Dialog object describing the standard PIN pad window
	 */
	public static Dialog createPinPad(XYPair winSize, byte frameBorderWidth, XYPair buttonSize, byte buttonBorderWidth, 
									int winColor, int frameColor, int frameBorderColor, 
									int buttonColor, int buttonBorderColor, int fontColor,
									XYPair logoSize, byte[] logoImage, int logoIndex)
	{
		ButtonSkin buttonSkin = ButtonSkin.create(buttonBorderWidth, buttonBorderColor, buttonColor, fontColor);
		RectangleSkin pinpadSkin = RectangleSkin.create(frameBorderWidth, frameBorderColor, frameColor);
		
		Dialog dialog = Dialog.create(winColor, winSize);
	
		if ( (logoSize.getX()) > 0 && (logoSize.getY() > 0) )
		{
			dialog.addWidget(getIntelLogoImage(logoImage,logoSize,logoIndex));
		}

		XYPair pinpadSize = getStandardFrameSize(winSize, logoSize);
		XYPair pinpadLoc = getStandardFrameLocation(winSize, logoSize);

		Widget pinpad = PinPadBuilder.createFixedPinPad(WIDGET_ID_PINPAD, pinpadSize, pinpadLoc,
														buttonSize, pinpadSkin, buttonSkin);

		dialog.addWidget(pinpad);
		
		return dialog;
	}

	/**
	 * Creates standard window with randomly located OK button.
	 * @param winSize 		the size of the window
	 * @param buttonSize 	the size of the OK button
	 * @param winColor 		window background color
	 * @param buttonColor 	OK button color
	 * @param logoSize 		size of the customized logo image
	 * @param logoImage 	customized logo image data
	 * @param logoIndex start index in the logo data array
	 * 
	 * @return Dialog object describing the standard OK button window
	 */
	public static Dialog createOKBox(XYPair winSize, byte frameBorderWidth, XYPair buttonSize, byte buttonBorderWidth,
									int winColor, int frameColor, int frameBorderColor, 
									int buttonColor, int buttonBorderColor, int fontColor,
									XYPair logoSize,  byte[] logoImage, int logoIndex)
	{
		ButtonSkin okButtonSkin = ButtonSkin.create(buttonBorderWidth, buttonBorderColor, buttonColor, fontColor);
		RectangleSkin okBoxSkin = RectangleSkin.create(frameBorderWidth, frameBorderColor, frameColor);

		Dialog dialog = Dialog.create(winColor, winSize);
	
		if ( (logoSize.getX()) > 0 && (logoSize.getY() > 0) )
		{
			dialog.addWidget(getIntelLogoImage(logoImage,logoSize,logoIndex));
		}

		// calculate the bounding box size
		XYPair okBoxSize = getStandardFrameSize(winSize, logoSize);
		XYPair okBoxLoc = getStandardFrameLocation(winSize, logoSize);

		Widget okBox = PinPadBuilder.createOkBox(WIDGET_ID_OKBUTTON, okBoxSize, okBoxLoc,
												buttonSize, okBoxSkin, okButtonSkin, OK_BOX_TEXT);

		dialog.addWidget(okBox);
		
		return dialog;
	}
	
	/**
	 * Creates standard CAPTCHA window.
	 * @param winSize the size of the window
	 * @param fontType the font of the text, see com.intel.ui.Label.FONT_TYPE_* definitions for possible values
	 * @param winColor window background color
	 * @param fontColor font color
	 * @param logoSize size of the customized logo image
	 * @param logoImage customized logo image data
	 * @param logoIndex start index in the logo data array
	 * @param captchaStr the text to be displayed to the user in ASCII format
	 * @return Dialog object describing the standard CAPTCHA window
	 */
	public static Dialog createCAPTCHA(XYPair winSize, byte frameBorderWidth, byte fontType,
									int winColor, int frameColor, int frameBorderColor, int fontColor, 
									XYPair logoSize,  byte[] logoImage, int logoIndex, String captchaStr)
	{
		if ( (captchaStr == null) || (captchaStr.length() == 0) || captchaStr.equals("") )
		{
			throw new UiIllegalParameterException();
		}

		Dialog dialog = Dialog.create(winColor, winSize); 

		if ( (logoSize.getX() > 0) && (logoSize.getY() > 0) )
		{
			dialog.addWidget(getIntelLogoImage(logoImage,logoSize,logoIndex));
		}

		// calculate the bounding box size
		XYPair captchaSize = getStandardFrameSize(winSize, logoSize);
		XYPair captchaLoc = getStandardFrameLocation(winSize, logoSize);
		
		Widget captcha = CaptchaBuilder.createCapthaPad(WIDGET_ID_CAPTCHA, captchaSize, captchaLoc,
														frameBorderWidth, frameColor, frameBorderColor,
														fontColor, fontType, captchaStr);
		dialog.addWidget(captcha);
		
		return dialog;
	}

	
	private static Image getIntelLogoImage(byte[] logo, XYPair logoSize, int offset)
	{
		if ( (logoSize.getX() < 0) || (logoSize.getY() < 0) )
		{
			throw new UiIllegalParameterException();
		}

		int imageLen = INTEL_LOGO_24BPP_PIXEL_SIZE * logoSize.getX() * logoSize.getY();
		if ( (logo.length - offset) < imageLen )
		{
			throw new UiIllegalParameterException();
		}
		
		byte[] imageBuffer = new byte[imageLen];
		
		ArrayUtils.copyByteArray(logo, offset, imageBuffer,0, imageLen);
		
		XYPair iconLoc = new XYPair(LOGO_MARGIN_X, LOGO_MARGIN_Y);
		
		return Image.create(WIDGET_ID_LOGO, logoSize, iconLoc, Image.FORMAT_RGB_24BPP, imageBuffer);
	}
	
}
