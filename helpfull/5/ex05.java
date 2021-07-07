package pkgEx05;

import com.intel.util.*;
import com.intel.crypto.SymmetricSignatureAlg;
import com.intel.util.Calendar;
import com.intel.util.TimeZone;
import com.intel.crypto.HashAlg;
import com.intel.crypto.RsaAlg;
import com.intel.langutil.ArrayUtils;


//
// Implementation of DAL Trusted Application: ex05_B_RonKeinan_203735857 
//
// **************************************************************************************************
// NOTE:  This default Trusted Application implementation is intended for DAL API Level 7 and above
// **************************************************************************************************

public class ex05 extends IntelApplet {

	
	//command const values:
    public final int CMD_GET_SEED = 7;
    public final int CMD_GET_ID = 8;
    public final int CMD_GET_OTP=9;
	
	public final int SUCCESS=1;
	public final int FAILURE=0;
	
    private static int counter = 1; //counter to calculate OTP, the server holds the same one
    

	
	
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
	
	
	
	//get integer and turn it to byte array by shifting right
	private byte[] intToByteArray(int v) {
		return new byte[] {(byte)v, (byte)(v>>8), (byte)(v>>16), (byte)(v >> 24)};
	}
	
	//get byte array and turn it to int by shifting left
	private int byteArrayToInt(byte[] b) {
		return ( ((b[3] & 0xFF)<<24) | ((b[2] & 0xFF)<<16) | ((b[1] & 0xFF)<<8) | ((b[0] & 0xFF)<<0) );
	}
	
	//compare 2 byte arrays
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
	
	
	//save seed to NVM storage  (FlashStorage java class)
	private boolean saveSeed(byte[] seed) {
		
		if (FlashStorage.getFlashDataSize(0) == 0) {		
		FlashStorage.writeFlashData(0, seed, 0, seed.length);
			return true;
		}
	
		return false;
	}
	
	//save user ID to NVM storage  (FlashStorage java class)
	private boolean saveID(byte[] id) {
		
		if (FlashStorage.getFlashDataSize(1) == 0) {		
		FlashStorage.writeFlashData(1, id, 0, id.length);
			return true;
		}
	
		return false;
	}
	
	//method to chain (combine) 2 byte arrays to 1 array
	private byte[] chainSeedCounter(byte[] longseed) {
		
		byte[] count = intToByteArray(counter); //turn to byte array
		
		byte[] seed = new byte[3]; //take only 3 digits (length of seed) - because more makes a different hash
		com.intel.langutil.ArrayUtils.copyByteArray(longseed, 0, seed, 0, 3); //copy to short byte array of 3 places
		
		byte[] united = new byte[seed.length + count.length];
		System.arraycopy(seed, 0, united, 0, seed.length);
		System.arraycopy(count, 0, united, seed.length, count.length);
		
		return united;
		
	}
	
	//method to create HMAC based OTP - using seed+counter as key, and SHA1 hashing algo
	private byte[] getOTP() {

		byte[] seed = new byte[256];
		FlashStorage.readFlashData(0, seed, 0); //take seed from flash
		
		
		HashAlg myHMAC = HashAlg.create(HashAlg.HASH_TYPE_SHA1); //create instance of Hash algo
		
		byte[] united = chainSeedCounter(seed); //create key to hash
		
		byte[] signature = new byte[256];
		myHMAC.processComplete(united, (short)0, (short)united.length, signature, (short)0); //hash the text
		
		counter++; //promote counter
		
		return signature;

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
	
			case (CMD_GET_SEED):
				if (saveSeed(request)==true) {
					returnCode= SUCCESS;
				}
				else {
					returnCode= FAILURE;
				}
			break;
			
			case (CMD_GET_ID):
				if (saveID(request)==true) {
					returnCode= SUCCESS;
				}
				else {
					returnCode= FAILURE;
				}
			break;	
			
			case (CMD_GET_OTP): {
					returnCode= SUCCESS;
					myResponse = getOTP();

				}
			break;	
			
			default:
				return APPLET_INVALID_COMMAND_ID;

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
