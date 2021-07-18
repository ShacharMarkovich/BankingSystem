/*++
   Copyright (c) 2010-2017 Intel Corporation. All Rights Reserved.

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

import com.intel.crypto.HashAlg;
import com.intel.crypto.Random;
import com.intel.crypto.RsaAlg;
import com.intel.crypto.SymmetricBlockCipherAlg;
import com.intel.langutil.ArrayUtils;
import com.intel.langutil.TypeConverter;
import com.intel.util.IntelApplet;
import com.intel.util.DebugPrint;
import com.intel.util.FlashStorage;
//
// Implementation of DAL Trusted Application: WYSApplet 
//
// ***************************************
// NOTE:  This Trusted Application is intended for API Level 2 and above
// ***************************************

public class WYS extends IntelApplet 
{
	static final int COMMAND_TEST_CONNECTION = 3;
	static final int COMMAND_ID_CHECK_INPUT_STATUS = 1;
	static final int COMMAND_ID_GET_OTP = 2;
	
	static final int COMMAND_GET_MODULUS = 5;
	static final int COMMAND_GET_EXPONENT = 6;
	static final int COMMAND_GET_ENCRYPTED_SESSION_KEY = 7;

	static final int COMMAND_ENCRYPT= 8;
	static final int COMMAND_DECRYPT= 9;
	
	static final int COMMAND_GEN_OTP = 10;
	
	static final int RESPONSE_TEST_CONNECTION = 0;
	static final int RESPONSE_OK = 1;

	
	private StandardWindow m_standardWindow;
	private RsaAlg rsa = null;
	private SymmetricBlockCipherAlg aes_cbc = null;
	private byte[] modulus;
	private byte[] exponent;
	private final byte[] PIN = {1, 7, 1, 7};
	final byte[] defualtResponse = { 'O', 'K' };
	
	private static int currFlashStorageIndex = 0;
	private static String loginUserID= "None";
	
	//get integer and turn it to byte array by shifting right
	private byte[] intToByteArray(int v) {
		return new byte[] {(byte)v, (byte)(v>>8), (byte)(v>>16), (byte)(v >> 24)};
	}
	
	//get byte array and turn it to int by shifting left
	private int byteArrayToInt(byte[] b) {
		return ( ((b[3] & 0xFF)<<24) | ((b[2] & 0xFF)<<16) | ((b[1] & 0xFF)<<8) | ((b[0] & 0xFF)<<0) );
	}
	
	private boolean byteArrayEquals(byte[] a, byte[] b)	{
		int i = 0;
		while (i < a.length && i < b.length) {
			if (a[i] != b[i])
				return false;
			i += 1;
		}
		return true;
	}
	
	/**
	 * after successful login, save the given new OTP secret key and user id in flash store
	 * @param data OTP secret key and user id
	 */
	private void saveOTPSecret(String data) {
		DebugPrint.printString("saveOTPSecret(byte[] data)");
		data = data.substring(5); // remove "1|id:"
		byte[] id = data.substring(0,data.indexOf("|")).getBytes(); // get new user id
		byte[] otpSecret = data.substring(data.indexOf("|") + 1).getBytes(); // get OTP secret key
		
		// save id, OTP key and counter in FlashStorage: 
		FlashStorage.writeFlashData(currFlashStorageIndex, id, 0, id.length);	
		currFlashStorageIndex++;
		FlashStorage.writeFlashData(currFlashStorageIndex, otpSecret, 0, otpSecret.length);	
		currFlashStorageIndex++;
		byte[] counter = intToByteArray(0);
		FlashStorage.writeFlashData(currFlashStorageIndex, counter, 0, counter.length);	
		currFlashStorageIndex++;
		DebugPrint.printString("finish saveOTPSecret");
	}
	
	/**
	 * Calc current login user's next OTP value 
	 * @return the OTP value
	 */
	private byte[] getOTPSecret() {
		byte[] userID = new byte[16];
		int i;
		for(i = 0; loginUserID.compareTo(new String(userID)) != 0; i += 3)
			FlashStorage.readFlashData(i, userID, 0);
		
		byte[] currOtpKey = new byte[64];
		FlashStorage.readFlashData(i+1, currOtpKey, 0);
		byte[] counter = new byte[4];
		FlashStorage.readFlashData(i+2, counter, 0);
				
		byte[] value = chainSeedCounter(counter,currOtpKey);
		HashAlg myHMAC = HashAlg.create(HashAlg.HASH_TYPE_SHA1); //create instance of Hash algo
		byte[] signature = new byte[256];
		myHMAC.processComplete(value, (short)0, (short)value.length, signature, (short)0); //hash the text
		encounter(i+2, byteArrayToInt(counter)+1);
		
		DebugPrint.printString("OTP: " + new String(signature));
		return signature;
	}
	
	/**
	 * increase the given counter value by one
	 * @param idex counter index
	 * @param newCount new value
	 */
	private void encounter(int idex, int newCount) {
		byte[] newC = intToByteArray(newCount);
		FlashStorage.writeFlashData(idex, newC , 0, newC.length);
	}
	
	private byte[] chainSeedCounter(byte[] count, byte[] secret) {
		byte[] united = new byte[secret.length + count.length];
		System.arraycopy(secret, 0, united, 0, secret.length);
		System.arraycopy(count, 0, united, secret.length, count.length);
		return united;
	}
	
	/**
	 * Extract the login user id, in order to know his fit OTP data later
	 * @param userData
	 */
	private void login(byte[] userData) {
		String data = new String(userData).substring(13); // remove "1|user login:"
		int startInd = data.indexOf("accNum\": ") + "accNum\": ".length(); // get user id's starting index 
		int endInd = data.indexOf("}"); // get user id's ending index
		loginUserID =  data.substring(startInd,endInd);
		DebugPrint.printString("loginUserID: " + loginUserID);
	}
	
	
	
	/*
	 * This method will be called by the VM when a new session is opened to the Trusted Application 
	 * and this Trusted Application instance is being created to handle the new session.
	 * This method cannot provide response data and therefore calling
	 * setResponse or setResponseCode methods from it will throw a NullPointerException.
	 * 
	 * @param	request	the input data sent to the Trusted Application during session creation
	 * 
	 * @return	APPLET_SUCCESS if the operation was processed successfully, 
	 * any other error status code otherwise (note that all error codes will be
	 * treated similarly by the VM by sending "cancel" error code to the SW application).
	 */
	public int onInit(byte[] request) {

		DebugPrint.printString("WYS applet entered");
		if (rsa == null) {
			rsa = RsaAlg.create();
			rsa.generateKeys((short)(128));
			
			modulus = new byte[rsa.getModulusSize()];
			exponent = new byte[rsa.getPublicExponentSize()];
			rsa.getKey(modulus,(short) 0, exponent,(short)0);
			rsa.setPaddingScheme(RsaAlg.PAD_TYPE_OAEP);
		}
		
		if(aes_cbc == null) {
			aes_cbc = SymmetricBlockCipherAlg.create(SymmetricBlockCipherAlg.ALG_TYPE_AES_CBC);
		}
		
		
		// the storages contains account id in even indexes and OTP fit secret key in following index and counter in the next one
		while (FlashStorage.getFlashDataSize(currFlashStorageIndex) != 0)
			currFlashStorageIndex += 3;
		
		m_standardWindow = StandardWindow.getInstance();
		
		return APPLET_SUCCESS;
	}
	
	/*
	 * This method will be called by the VM to handle a command sent to this
	 * Trusted Application instance.
	 * 
	 * @param	commandID	the command ID (Trusted Application specific) 
	 * @param	request		the input data for this command 
	 * @return	the return value should not be used by the applet
	 */
	public int invokeCommand(int commandID, byte[] request) {
		int res = IntelApplet.APPLET_ERROR_NOT_SUPPORTED;
		switch (commandID)
		{
			case StandardWindow.STANDARD_COMMAND_ID:
				
				DebugPrint.printString("Processing standard command...");
				res = m_standardWindow.processCommand(commandID, request, 0);
				if ( (res == IntelApplet.APPLET_SUCCESS) &&
					 (m_standardWindow.getResponseSize() > 0) )
				{
					byte[] response = new byte[m_standardWindow.getResponseSize()];
					m_standardWindow.getResponse(response, 0);
					setResponse(response, 0, response.length);
				}
				break;
				
			case COMMAND_ID_CHECK_INPUT_STATUS:
				
				DebugPrint.printString("Checking user input status...");
				if ( isOtpAllowed() )
				{
					DebugPrint.printString("User input is OK");
					res = IntelApplet.APPLET_SUCCESS;
				}
				else
				{
					DebugPrint.printString("User input is wrong");
					res = IntelApplet.APPLET_ERROR_BAD_PARAMETERS;
				}
				break;
			
			case COMMAND_ID_GET_OTP:
				
				DebugPrint.printString("Getting OTP...");
				res = getOtp(); 
				break;
			case COMMAND_TEST_CONNECTION:
				
				if(request != null)
				{
					DebugPrint.printString("Received buffer:");
					DebugPrint.printBuffer(request);
				}
				
				setResponse(defualtResponse, 0, defualtResponse.length); // return OK
				res = RESPONSE_TEST_CONNECTION;
				break;
				
			case COMMAND_GET_MODULUS:
				setResponse(modulus,0,modulus.length);
				DebugPrint.printString("get modulus");
				res = RESPONSE_OK;
				break;
			case COMMAND_GET_EXPONENT:
				setResponse(exponent,0,exponent.length);
				DebugPrint.printString("get exponent");
				res = RESPONSE_OK;
				break;
			
			case COMMAND_GET_ENCRYPTED_SESSION_KEY:
				byte[] data = new byte[modulus.length];
				rsa.decryptComplete(request, (short)0, (short)modulus.length, data,(short) 0);
								
				aes_cbc.setKey(data, (short)0, (short)16);
				aes_cbc.setIV(data, (short)16, (short)16);
				
				setResponse(defualtResponse, 0, defualtResponse.length); // return OK
				res = RESPONSE_OK;
				DebugPrint.printString("dec shared key compete");
				break;
			
			case COMMAND_ENCRYPT:
				byte[] encMsg = new byte[4096];
				
				aes_cbc.encryptComplete(request, (short)0, (short)request.length, encMsg, (short)0);
				DebugPrint.printString("encryptComplete");
				setResponse(encMsg, 0, encMsg.length);
				res = RESPONSE_OK;
				break;
			
			case COMMAND_DECRYPT:
				byte[] decMsg = new byte[request.length];
				DebugPrint.printString("before decryptComplete");
				aes_cbc.decryptComplete(request, (short)0, (short)request.length, decMsg, (short)0);
				DebugPrint.printString("after decryptComplete");
				String response = new String(decMsg);
				if (response.startsWith("1|id:"))
				{
					DebugPrint.printInt(response.length());
					saveOTPSecret(response);
					decMsg = "1|user created".getBytes();
					setResponse(decMsg, 0, decMsg.length);
				}
				else if (response.startsWith("1|user login:"))
				{
					login(decMsg);
					setResponse(decMsg, 0, decMsg.length);
				}
				else
					setResponse(decMsg, 0, decMsg.length);
				res = RESPONSE_OK;
				break;
				
			case COMMAND_GEN_OTP:
				byte[] signature = getOTPSecret();
				setResponse(signature, 0, signature.length);
				res = RESPONSE_OK;
				break;
				
			default:
				break;
		}
		
		setResponseCode(res);
		
		/*
		 * The return value of the invokeCommand method is not guaranteed to be
		 * delivered to the SW application, and therefore should not be used for
		 * this purpose. Trusted Application is expected to return APPLET_SUCCESS code 
		 * from this method and use the setResposeCode method instead.
		 */
		return APPLET_SUCCESS;
	}
	
	private int getOtp()
	{
		if ( !isOtpAllowed() )
		{
			DebugPrint.printString("OTP is blocked.");
			return IntelApplet.APPLET_ERROR_BAD_STATE;
		}
		
		byte[] otp = new byte[TypeConverter.INT_BYTE_SIZE];
		Random.getRandomBytes(otp, (short)0, (short)otp.length);
		
		setResponse(otp, 0, otp.length);
		
		return IntelApplet.APPLET_SUCCESS;
	}

	private boolean isOtpAllowed()
	{
		if ( m_standardWindow.getUserInputStatus() == true )
		{
			return true;
		}
		
		// try to check PIN number
		byte[] userPIN = m_standardWindow.getPin();
		
		if ( (userPIN != null) && (userPIN.length == PIN.length) && (ArrayUtils.compareByteArray(userPIN, 0, PIN, 0, PIN.length)) )
		{
			return true;
		}
		
		return false;
	}
	
	/*
	 * This method will be called by the VM when the session being handled by
	 * this Trusted Application instance is being closed 
	 * and this Trusted Application instance is about to be removed.
	 * This method cannot provide response data and therefore
	 * calling setResponse or setResponseCode methods from it will throw a NullPointerException.
	 * 
	 * @return APPLET_SUCCESS code (the status code is not used by the VM).
	 */
	public int onClose() {
		DebugPrint.printString("WYS applet exited");
		return APPLET_SUCCESS;
	}
}