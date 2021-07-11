using System;
using System.Windows.Forms;
using Newtonsoft.Json;
using Client;

namespace WysHost
{
    public partial class LoginWin : Form
    {
        public LoginWin()
        {
            InitializeComponent();
            passwordTextBox.PasswordChar = '*';
        }

        private void retBtn_Click(object sender, EventArgs e) => Close();

        private void loginBtn_Click(object sender, EventArgs e)
        {
            if (usernameTextBox.Text != "" && passwordTextBox.Text != "")
            {
                var loginData = new { username = usernameTextBox.Text, hash_password = Utils.sha256_hash(passwordTextBox.Text) };
                string jsonString = JsonConvert.SerializeObject(loginData);
                MessageBox.Show(jsonString);
                string respone = MainWin.connector.SendAndRecvServer(serverOpcode.login, jsonString);
                if (respone[0] == Utils.SUCCESSED)
                {
                    MessageBox.Show(respone);
                    Account acc = JsonConvert.DeserializeObject<Account>(respone.Substring(2));
                    new AccountPage(acc).ShowDialog();
                }
                else
                    MessageBox.Show(respone.Substring(2));
            }
            else
                MessageBox.Show("Please fill both username & password!");

            usernameTextBox.Text = "";
            passwordTextBox.Text = "";
        }
    }
}
