using Client;
using Newtonsoft.Json;
using System.Windows.Forms;

namespace WysHost.Windows
{
    public partial class TransferWin : Form
    {
        public TransferWin()
        {
            InitializeComponent();
        }

        private void submitBtn_Click(object sender, System.EventArgs e)
        {
            int accNum;
            int amount;
            if (int.TryParse(_amountInput.Text, out amount) && int.TryParse(_accNumTB.Text, out accNum))
            {
                if (accNum > 0 && amount > 0)
                {
                    var loginData = new { accNum = accNum.ToString(), amount = amount.ToString() };
                    string jsonString = JsonConvert.SerializeObject(loginData);
                    string res = MainWin.connector.SendAndRecvServer(serverOpcode.transfer, jsonString);
                    if (res[0] == Utils.SUCCESSED)
                        AccountPage._currAccount.Balance -= amount;
                    MessageBox.Show(res.Substring(2));
                }
                else
                    MessageBox.Show("Do not be a wiseguy.");
            }
            else
                MessageBox.Show("please enter a number!");
        }

        private void cancelBtn_Click(object sender, System.EventArgs e) => Close();
    }
}
