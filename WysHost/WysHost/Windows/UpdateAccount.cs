using Client;
using Newtonsoft.Json;
using System;
using System.Windows.Forms;

namespace WysHost
{
    public partial class UpdateAccount : Form
    {
        Account _currAccount;

        public UpdateAccount(Account acc)
        {
            InitializeComponent();
            _currAccount = acc;
            accountBindingSource.DataSource = _currAccount;
            passwordTextBox.UseSystemPasswordChar = true;
        }

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
        private void UpdateAccountData()
        {
            _currAccount.Username = usernameTextBox.Text;
            _currAccount.Email = emailTextBox.Text;
            _currAccount.FullName = fullNameTextBox.Text;
            _currAccount.BirthDay = birthDayDateTimePicker.Value.Date;
            _currAccount.Country = countryTextBox.Text;
            _currAccount.City = cityTextBox.Text;
            _currAccount.Street = streetTextBox.Text;
            _currAccount.Gender = genderComboBox.SelectedItem as string;
            _currAccount.IsMarry = isMarryCheckBox.Checked;
            int num = -1;
            int.TryParse(houseNumTextBox.Text, out num);
            _currAccount.HouseNum = num;
        }

        /// <summary>
        /// check if each property is valid
        /// </summary>
        /// <param name="newAcc">account to check</param>
        /// <returns>if all properties are valid</returns>
        private bool CheckNewAccountData(Account newAcc)
        {
            return newAcc.Username != "" && IsValidEmail(newAcc.Email) && newAcc.FullName != "" &&
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
            UpdateAccountData();
            if (CheckNewAccountData(_currAccount))
            {
                string jsonString = JsonConvert.SerializeObject(_currAccount);
                string respone = MainWin.connector.SendAndRecvServer(serverOpcode.updateData, jsonString);
                MessageBox.Show(respone.Substring(2));
                if (respone[0] == Utils.SUCCESSED)
                    Close();
            }
            else MessageBox.Show("not all data had been writen legally!\nFix it!");
        }

        private void clearBtn_Click(object sender, EventArgs e) => accountBindingSource.Clear();

        private void retBtn_Click(object sender, EventArgs e) => Close();
    }
}
