package pkgEx04;

import com.intel.util.*;
import com.intel.crypto.RsaAlg;
import com.intel.langutil.ArrayUtils;

//
// Implementation of DAL Trusted Application: ex04_RonKeinan 
//
// **************************************************************************************************
// NOTE:  This default Trusted Application implementation is intended for DAL API Level 7 and above
// **************************************************************************************************

public class ex04_RonKeinan extends IntelApplet {

	/**
	 * This method will be called by the VM when a new session is opened to the Trusted Application 
	 * and this Trusted Application instance is being created to handle the new session.
	 * This method cannot provide response data and therefore calling
	 * setResponse or setResponseCode methods from it will throw a NullPointerException.
	 * 
	 * @param	request	the input data sent to the Trusted Application during session creation
	 * 
	 * @return	APPLET_SUCCESS if the operation was processed successfully, 
	 * 		any other error status code otherwise (note that all error codes will be
	 * 		treated similarly by the VM by sending "cancel" error code to the SW application).
	 */
	public int onInit(byte[] request) {
		DebugPrint.printString("Hello, DAL!");
		return APPLET_SUCCESS;
	}
	

	
    /*cmd ID:
     * 1 = set password
     * 2 = login
     * 3 = reset password
     * 4 = get account info
     * 5 = logout
     * 6 = generate key pair
	 * 7 = get public key
     * 8 = send message, get it coded and verify
     */
	public final int CMD_SET=1;
	public final int CMD_LOGIN=2;
	public final int CMD_RESET=3;
	public final int CMD_GET=4;
	public final int CMD_LOGOUT=5;
	public final int CMD_GENERATE_KEY=6;
	public final int CMD_GET_PUBLIC_KEY=7;
	public final int CMD_SEND_AND_VERIFY=8;
	
	public final int SUCCESS=1;
	public final int FAILURE=0;

	
	public boolean isLogged = false;

	
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
			
	            if (a[i] != b[i]) {
	            	return false;
	            }
	            i += 1;
	      }

	      return true;

	}
	
	//save password to NVM storage  (FlashStorage java class)
	private boolean setPassword(byte[] pass) {
		
		if (FlashStorage.getFlashDataSize(0) == 0) {		
		FlashStorage.writeFlashData(0, pass, 0, pass.length);
			return true;
		}
	
		return false;

	}

	//return true IFF password matches. And creates a session.(note, this is a log-in session not the applet session).
	private boolean login(byte[] pass) {
		if (isLogged == true) {
			return true;
		}

		if (FlashStorage.getFlashDataSize(0) != 0) {
			
			byte[] dest = new byte[256];
			FlashStorage.readFlashData(0, dest, 0);
			
			if (ArrayUtils.compareByteArray(pass, 0, dest, 0, pass.length)) {

				isLogged = true;
				return true;
			}		
		}

			return false;	//there is no password or password is not correct
	}

	// works if session i\s active (Log-in need to be called before). reset password from old to new.
	private boolean resetPassword(byte[] pass) {
		
		if (isLogged == true) {
			FlashStorage.eraseFlashData(0);
			//return setPassword(pass);
			FlashStorage.writeFlashData(0, pass, 0, pass.length);
			return true;
		}
		
		return false;

	}

	// return student name
	private byte[] getAcountInfo() {
		
		if (isLogged == true) {
			byte[] resp2 = { 'R', 'O', 'N' };
			return resp2;

		}
		
		byte[] resp1 = { 'N', 'U', 'L', 'L' };
		return resp1;

	}
	
	private boolean logout() {
		
		if (isLogged == true) {
			isLogged = false;
			return true;
		}
		
		return false;
		
	}
	
	//create a new instance of RSA library, set hash algorithm sha256 and padding pkcs1,
	//generate modulus(256 bytes), public key(4 bytes) and private key(256 bytes) and keep to flash storage (in 1 byte array
	private boolean generateKey() {
		if ( (isLogged == true) && (FlashStorage.getFlashDataSize(1) == 0) ) {
			RsaAlg myRSA = RsaAlg.create();
			myRSA.setHashAlg(RsaAlg.HASH_TYPE_SHA256);
			myRSA.setPaddingScheme(RsaAlg.PAD_TYPE_PKCS1);
			myRSA.generateKeys((short) 256);
			
			byte[] modulus = new byte[256];
			byte[] publicKey = new byte[256];
			byte[] privateKey = new byte[256];
			myRSA.getKey(modulus, (short) 0, publicKey, (short) 0, privateKey, (short) 0);
			
			byte[] keys =new byte[768];
			
			for (int i=0; i<256; i++) { //unite to 1 array for flash storage
				keys[i] = modulus[i];
				keys[i+256] = publicKey[i];
				keys[i+512] = privateKey[i];
			}

			FlashStorage.writeFlashData(1, keys, 0, keys.length);
			
			return true;
		}
		return false;
		
	}
	
	//check if keys already created and return 
	private byte[] getKeys() {
		if ( (isLogged == true)  && (FlashStorage.getFlashDataSize(1) != 0) ) {
				byte[] keys = new byte[768];
				FlashStorage.readFlashData(1, keys, 0);
				
				byte [] toReturn = new byte[768];
				ArrayUtils.copyByteArray(keys, 0, toReturn, 0, 260); //copy without the public key
				return toReturn;
		}
		byte[] resp = { 'N', 'U', 'L', 'L' };
		return resp;
	}

	//get a word from host, sign it with private key and send the signature to host.
	private byte[] signAndVerify(byte[] data) {
		if ( (isLogged == true)  && (FlashStorage.getFlashDataSize(1) != 0) ) {
			byte[] keys = new byte[1024];
			FlashStorage.readFlashData(1, keys, 0);
			
			RsaAlg myRSA = RsaAlg.create();
			myRSA.setHashAlg(RsaAlg.HASH_TYPE_SHA256);
			myRSA.setPaddingScheme(RsaAlg.PAD_TYPE_PKCS1);
			myRSA.generateKeys((short) 256);
			myRSA.setKey(keys, (short) 0, (short) 256, keys, (short) 256, (short) 4, keys, (short) 512, (short) 256);
			
			byte[] signature = new byte[256];
			int ret = myRSA.signComplete(data, (short) 0, (short) data.length, signature, (short) 0);
			if (ret>0) {
				return signature;
			}
		}
		byte[] resp = { 'N', 'U', 'L', 'L' };
		return resp;
	}
	
	/**
	 * This method will be called by the VM to handle a command sent to this
	 * Trusted Application instance.
	 * 
	 * @param	commandId	the command ID (Trusted Application specific) 
	 * @param	request		the input data for this command 
	 * @return	the return value should not be used by the applet
	 */
	public int invokeCommand(int commandId, byte[] request) {
			
			int returnCode=0; //
			
			byte[] myResponse = { 'O', 'K' };

		
		switch (commandId) {
		
				case (CMD_SET):
					if (setPassword(request)==true) {
						returnCode= SUCCESS;
					}
					else {
						returnCode= FAILURE;
					}
				break;
				
				case (CMD_LOGIN):
					if (login(request)==true) {
						returnCode= SUCCESS;
					}
					else {
						returnCode= FAILURE;
					}
				break;				
				
				case (CMD_RESET):
					if (resetPassword(request)==true) {
						returnCode= SUCCESS;
					}
					else {
						returnCode= FAILURE;
					}
				break;
				
				case (CMD_GET):
					myResponse = getAcountInfo();
				break;	
				
				case (CMD_LOGOUT):
					if (logout()==true) {
						returnCode= SUCCESS;
					}
					else {
						returnCode= FAILURE;
					}
				break;	
				
				case (CMD_GENERATE_KEY):
					if (generateKey()==true) {
						returnCode= SUCCESS;
					}
					else {
						returnCode= FAILURE;
					}
				break;	
				
				case (CMD_GET_PUBLIC_KEY):
					myResponse = getKeys();
					returnCode= SUCCESS;
				break;	
				
				case (CMD_SEND_AND_VERIFY):
					myResponse = signAndVerify(request);
					returnCode= SUCCESS;
				break;	
		}
		
				
		
		/*
		 * To return the response data to the command, call the setResponse
		 * method before returning from this method. 
		 * Note that calling this method more than once will 
		 * reset the response data previously set.
		 */
		setResponse(myResponse, 0, myResponse.length);

		/*
		 * In order to provide a return value for the command, which will be
		 * delivered to the SW application communicating with the Trusted Application,
		 * setResponseCode method should be called. 
		 * Note that calling this method more than once will reset the code previously set. 
		 * If not set, the default response code that will be returned to SW application is 0.
		 */
		setResponseCode(returnCode);

		/*
		 * The return value of the invokeCommand method is not guaranteed to be
		 * delivered to the SW application, and therefore should not be used for
		 * this purpose. Trusted Application is expected to return APPLET_SUCCESS code 
		 * from this method and use the setResposeCode method instead.
		 */
		return APPLET_SUCCESS;
	}

	/**
	 * This method will be called by the VM when the session being handled by
	 * this Trusted Application instance is being closed 
	 * and this Trusted Application instance is about to be removed.
	 * This method cannot provide response data and therefore
	 * calling setResponse or setResponseCode methods from it will throw a NullPointerException.
	 * 
	 * @return APPLET_SUCCESS code (the status code is not used by the VM).
	 */
	public int onClose() {
		DebugPrint.printString("Goodbye, DAL!");
		return APPLET_SUCCESS;
	}
}
