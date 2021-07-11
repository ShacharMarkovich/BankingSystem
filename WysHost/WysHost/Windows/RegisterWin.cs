using Client;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WysHost
{
    public partial class RegisterWin : Form
    {
        public RegisterWin()
        {
            InitializeComponent();
            showPassBtn.BackgroundImage = Image.FromFile("../../Images/show.jpg");
            showPassBtn.BackgroundImageLayout = ImageLayout.Stretch;
            passwordTextBox.UseSystemPasswordChar = true;
        }

        private void retBtn_Click(object sender, EventArgs e) => Close();

        private void clearBtn_Click(object sender, EventArgs e) => accountBindingSource.Clear();


        #region Validation check functions

        /// <summary>
        /// check if given Email is in the correct format and with real TLD
        /// </summary>
        /// <param name="email">email to check</param>
        /// <returns>if email is valid</returns>
        bool IsValidEmail(string email)
        {
            try
            {
                var addr = new System.Net.Mail.MailAddress(email);
                return addr.Address == email;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// get the new Account details from GUI
        /// </summary>
        /// <returns>the new account</returns>
        private Account GetNewAccountData()
        {
            Account newAcc = new Account();
            newAcc.Username = usernameTextBox.Text;
            newAcc.Password = passwordTextBox.Text;
            newAcc.Email = emailTextBox.Text;
            newAcc.FullName = fullNameTextBox.Text;
            newAcc.BirthDay = birthDayDateTimePicker.Value.Date;
            newAcc.Country = countryTextBox.Text;
            newAcc.City = cityTextBox.Text;
            newAcc.Street = streetTextBox.Text;
            newAcc.Gender = genderComboBox.SelectedItem as string;
            newAcc.IsMarry = isMarryCheckBox.Checked;
            int num = -1;
            int.TryParse(houseNumTextBox.Text, out num);
            newAcc.HouseNum = num;

            return newAcc;
        }

        /// <summary>
        /// check if each property is valid
        /// </summary>
        /// <param name="newAcc">account to check</param>
        /// <returns>if all properties are valid</returns>
        private bool CheckNewAccountData(Account newAcc)
        {
            return newAcc.Username != "" && newAcc.Password != "" && IsValidEmail(newAcc.Email) && newAcc.FullName != "" &&
                newAcc.Country != "" && newAcc.City != "" && newAcc.Street != "" && newAcc.HouseNum > 0 &&
                (newAcc.Gender == "Male" || newAcc.Gender == "Female");
        }
        #endregion

        /// <summary>
        /// event handle the creation of new account
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void submitBtn_Click(object sender, EventArgs e)
        {
            Account newAcc = GetNewAccountData();
            if (CheckNewAccountData(newAcc))
            {
                // Serialize details and send to server and TA
                newAcc.Password = Utils.sha256_hash(newAcc.Password);
                string jsonString = JsonConvert.SerializeObject(newAcc);
                string response = MainWin.connector.SendAndRecvServer(serverOpcode.register, jsonString);

                MessageBox.Show(response.Substring(2)); // show response msg
                if (response[0] == Utils.SUCCESSED)
                    Close(); // if creation work - close window and move back to main window
                else // error - clean details
                {
                    usernameTextBox.Text = "";
                    passwordTextBox.Text = "";
                    emailTextBox.Text = "";
                }
            }
            else
                MessageBox.Show("not all data had been writen legally!\nFix it!");
        }

        /// <summary>
        /// event hadle the clicking on show/hide password
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void showPassBtn_Click(object sender, EventArgs e)
        {
            if (passwordTextBox.UseSystemPasswordChar)
                showPassBtn.BackgroundImage = Image.FromFile("../../Images/hide.jpg");
            else
                showPassBtn.BackgroundImage = Image.FromFile("../../Images/show.jpg");

            passwordTextBox.UseSystemPasswordChar = !passwordTextBox.UseSystemPasswordChar;
        }
    }
}
