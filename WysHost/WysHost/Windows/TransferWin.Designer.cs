namespace WysHost.Windows
{
    partial class TransferWin
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.cancelBtn = new System.Windows.Forms.Button();
            this.submitBtn = new System.Windows.Forms.Button();
            this._amountInput = new System.Windows.Forms.TextBox();
            this.amountLbl = new System.Windows.Forms.Label();
            this._accNumTB = new System.Windows.Forms.TextBox();
            this.accNumLbl = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // cancelBtn
            // 
            this.cancelBtn.Location = new System.Drawing.Point(167, 159);
            this.cancelBtn.Name = "cancelBtn";
            this.cancelBtn.Size = new System.Drawing.Size(75, 23);
            this.cancelBtn.TabIndex = 7;
            this.cancelBtn.Text = "Cancel";
            this.cancelBtn.UseVisualStyleBackColor = true;
            this.cancelBtn.Click += new System.EventHandler(this.cancelBtn_Click);
            // 
            // submitBtn
            // 
            this.submitBtn.Location = new System.Drawing.Point(43, 159);
            this.submitBtn.Name = "submitBtn";
            this.submitBtn.Size = new System.Drawing.Size(75, 23);
            this.submitBtn.TabIndex = 6;
            this.submitBtn.Text = "Submit";
            this.submitBtn.UseVisualStyleBackColor = true;
            this.submitBtn.Click += new System.EventHandler(this.submitBtn_Click);
            // 
            // _amountInput
            // 
            this._amountInput.Location = new System.Drawing.Point(142, 105);
            this._amountInput.Name = "_amountInput";
            this._amountInput.Size = new System.Drawing.Size(100, 20);
            this._amountInput.TabIndex = 5;
            // 
            // amountLbl
            // 
            this.amountLbl.AutoSize = true;
            this.amountLbl.Location = new System.Drawing.Point(40, 108);
            this.amountLbl.Name = "amountLbl";
            this.amountLbl.Size = new System.Drawing.Size(46, 13);
            this.amountLbl.TabIndex = 4;
            this.amountLbl.Text = "Amount:";
            // 
            // _accNumTB
            // 
            this._accNumTB.Location = new System.Drawing.Point(142, 69);
            this._accNumTB.Name = "_accNumTB";
            this._accNumTB.Size = new System.Drawing.Size(100, 20);
            this._accNumTB.TabIndex = 9;
            // 
            // accNumLbl
            // 
            this.accNumLbl.AutoSize = true;
            this.accNumLbl.Location = new System.Drawing.Point(40, 72);
            this.accNumLbl.Name = "accNumLbl";
            this.accNumLbl.Size = new System.Drawing.Size(90, 13);
            this.accNumLbl.TabIndex = 8;
            this.accNumLbl.Text = "Account Number:";
            // 
            // TransferWin
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 261);
            this.Controls.Add(this._accNumTB);
            this.Controls.Add(this.accNumLbl);
            this.Controls.Add(this.cancelBtn);
            this.Controls.Add(this.submitBtn);
            this.Controls.Add(this._amountInput);
            this.Controls.Add(this.amountLbl);
            this.Name = "TransferWin";
            this.Text = "Transfer Window";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button cancelBtn;
        private System.Windows.Forms.Button submitBtn;
        private System.Windows.Forms.TextBox _amountInput;
        private System.Windows.Forms.Label amountLbl;
        private System.Windows.Forms.TextBox _accNumTB;
        private System.Windows.Forms.Label accNumLbl;
    }
}