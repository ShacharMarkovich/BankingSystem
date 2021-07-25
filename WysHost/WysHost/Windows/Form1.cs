/**
***
*** Copyright  (C) 2013-2014 Intel Corporation. All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation. and may not be disclosed, examined
*** or reproduced in whole or in part without explicit written authorization
*** from the company.
***
*** ----------------------------------------------------------------------------
**/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;
using System.Threading;
using Client;

namespace WysHost
{
    public partial class Form1 : Form
    {
        
        public Form1()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Gets the selected WYS image type from the GUI radio buttons.
        /// </summary>
        /// <returns>One of the LogicInterface.WYS_IMAGE_TYPE_ constants.</returns>
        private Byte getWysImageType()
        {
            Byte wysImageType = 0;
            if (radioButton_pinPad.Checked)
            {
                wysImageType = WysWrapper.WYS_IMAGE_TYPE_PINPAD;
            }
            else if (radioButton_okButton.Checked)
            {
                wysImageType = WysWrapper.WYS_IMAGE_TYPE_OKBUTTON;
            }
            else if (radioButton_captcha.Checked)
            {
                wysImageType = WysWrapper.WYS_IMAGE_TYPE_CAPTCHA;
            }

            return wysImageType;
        }

        /// <summary>
        /// Performs initialization of the JHI/WYS libraries and renders 
        /// the WYS image using the selected radio button as the image type.
        /// </summary>
        private void button_initAndRender_Click(object sender, EventArgs e)
        {
            //Grey out the GUI:
            radioButton_captcha.Enabled = false;
            radioButton_okButton.Enabled = false;
            radioButton_pinPad.Enabled = false;

            if (radioButton_captcha.Checked)
            {
                label_captchaInput.Visible = true;
                textBox_captchaInput.Visible = true;
            }
            else
            {
                label_captchaInput.Visible = false;
                textBox_captchaInput.Visible = false;
            }

            UInt32 ret = WysWrapper.doWysSequence(panel_wysView.Handle, getWysImageType());
            alertOnFailure(ret, WysWrapper.SAMPLE_CODE_SUCCESS, "WYS", "WYS operations failed");
        }

        /// <summary>
        /// Handles mouse-down events on the WYS image area.
        /// </summary>
        private void panel_wysView_MouseDown(object sender, MouseEventArgs e)
        {
            UInt16 x = (UInt16)e.X;
            UInt16 y = (UInt16)e.Y;
            bool ret = WysWrapper.onMouseDown(panel_wysView.Handle, x, y);
        }

        /// <summary>
        /// Handles mouse-up events on the WYS image area.
        /// </summary>
        private void panel_wysView_MouseUp(object sender, MouseEventArgs e)
        {
            UInt16 x = (UInt16)e.X;
            UInt16 y = (UInt16)e.Y;
            bool ret = WysWrapper.onMouseUp(x, y);
        }

        /// <summary>
        /// Handles a "submit" request. The request will be passed on to the WYS library.
        /// Exact behavior depends on the selected WYS image type.
        /// </summary>
        private void button_submit_Click(object sender, EventArgs e)
        {
            //int unixT = (int)(DateTime.UtcNow.Subtract(new DateTime(1970, 1, 1))).TotalSeconds / Utils.INTERVAL;
            bool ret;
            if(getWysImageType() == WysWrapper.WYS_IMAGE_TYPE_CAPTCHA)
            {
                string userInput = textBox_captchaInput.Text;
                UInt16 inputLength = (UInt16)userInput.Length;
                //To be called on "submit" events, when in CAPTCHA mode. The user's input is passed as parameter.
                ret = WysWrapper.onClickSubmit(userInput, inputLength);
            }
            else
            {
                //To be called on "submit" events. Not to be used in CAPTCHA mode.
                ret = WysWrapper.onClickSubmit(null, 0);
            }
            const int otpLength = 4;
            byte[] otpBytes = new byte[otpLength];
            IntPtr outArr = Marshal.AllocHGlobal(otpLength);
            if (WysWrapper.getOtp(outArr, otpLength))
            {
                Marshal.Copy(outArr, otpBytes, 0, otpLength);
                Marshal.FreeHGlobal(outArr);

                string otp = BitConverter.ToString(otpBytes).Replace("-", "");
                // TODO: send to encrtpe and then to server, is good msg come back - done, alse, close windows and try again.
                MessageBox.Show(string.Format("Success! The generated OTP is: {0}", otp));
            }
            else
            {
                MessageBox.Show("Failed to verify user input.");
            }

            radioButton_captcha.Visible = true;
            radioButton_okButton.Visible = true;
            radioButton_pinPad.Visible = true;
            radioButton_pinPad.Checked = true;
            textBox_captchaInput.Text = null;
#if AMULET
            radioButton_captcha.Enabled = true;
            radioButton_okButton.Enabled = true;
            radioButton_pinPad.Enabled = true;         
#else
        radioButton_captcha.Enabled = false;
            radioButton_okButton.Enabled = false;
            radioButton_pinPad.Enabled = false;
            button_clear.Enabled = false;
            button_initAndRender.Enabled = false;
            button_submit.Enabled = false;
#endif
        }

        /// <summary>
        /// Handles a "clear" request. The request will be passed on to the WYS library.
        /// Exact behavior depends on the selected WYS image type.
        /// </summary>
        private void button_clear_Click(object sender, EventArgs e)
        {
            if (getWysImageType() == WysWrapper.WYS_IMAGE_TYPE_CAPTCHA)
            {
                MessageBox.Show("This operation is not supported in CAPTCHA mode.");
            }
            else
            {
                bool ret = WysWrapper.onClickClear();
                String msg;

                if (!ret)
                {
                    msg = "Success!";
                }
                else
                {
                    msg = "Failed!";
                }
                MessageBox.Show(msg);
            }
        }

        /// <summary>
        /// Called when the form is closing. De-inits the libraries used.
        /// </summary>
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Hide();
            WysWrapper.Close();
            Application.Exit();
        }

        /// <summary>
        /// Checks if errorCode is not successErrorCodeToCompareTo. If they are different, 
        /// a message box is displayed, with the text "Error: "+errorDescription+"!\n"+errorProviderName+" code = 0x[errorCode, in hex]".
        /// </summary>
        private void alertOnFailure(UInt32 errorCode, UInt32 successErrorCodeToCompareTo, string errorProviderName, string errorDescription)
        {
            if (errorCode != successErrorCodeToCompareTo)
            {
                MessageBox.Show(String.Format("Error: " + errorDescription + "!\n" + errorProviderName + " code = 0x{0:X}", errorCode));
            }
        }

        private void radioButton_okButton_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_pinPad.Checked)
            {
                label_captchaInput.Visible = false;
                textBox_captchaInput.Visible = false;
            }
            else if (radioButton_okButton.Checked)
            {
                label_captchaInput.Visible = false;
                textBox_captchaInput.Visible = false;
            }
            else if (radioButton_captcha.Checked)
            {
                label_captchaInput.Visible = true;
                textBox_captchaInput.Visible = true;
            }
        }

    }
}
