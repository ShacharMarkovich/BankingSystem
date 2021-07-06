namespace WysHost
{
    partial class Form1
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
            this.panel_wysView = new System.Windows.Forms.Panel();
            this.radioButton_pinPad = new System.Windows.Forms.RadioButton();
            this.radioButton_captcha = new System.Windows.Forms.RadioButton();
            this.radioButton_okButton = new System.Windows.Forms.RadioButton();
            this.button_initAndRender = new System.Windows.Forms.Button();
            this.label_captchaInput = new System.Windows.Forms.Label();
            this.textBox_captchaInput = new System.Windows.Forms.TextBox();
            this.button_submit = new System.Windows.Forms.Button();
            this.button_clear = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            this.SuspendLayout();
            // 
            // panel_wysView
            // 
            this.panel_wysView.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel_wysView.Location = new System.Drawing.Point(46, 73);
            this.panel_wysView.Name = "panel_wysView";
            this.panel_wysView.Size = new System.Drawing.Size(260, 260);
            this.panel_wysView.TabIndex = 1;
            this.panel_wysView.MouseDown += new System.Windows.Forms.MouseEventHandler(this.panel_wysView_MouseDown);
            this.panel_wysView.MouseUp += new System.Windows.Forms.MouseEventHandler(this.panel_wysView_MouseUp);
            // 
            // radioButton_pinPad
            // 
            this.radioButton_pinPad.AutoSize = true;
            this.radioButton_pinPad.BackColor = System.Drawing.Color.White;
            this.radioButton_pinPad.Checked = true;
            this.radioButton_pinPad.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(192)))), ((int)(((byte)(255)))));
            this.radioButton_pinPad.FlatAppearance.BorderSize = 2;
            this.radioButton_pinPad.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(177)));
            this.radioButton_pinPad.ForeColor = System.Drawing.Color.MidnightBlue;
            this.radioButton_pinPad.Location = new System.Drawing.Point(19, 347);
            this.radioButton_pinPad.Name = "radioButton_pinPad";
            this.radioButton_pinPad.Size = new System.Drawing.Size(65, 17);
            this.radioButton_pinPad.TabIndex = 1;
            this.radioButton_pinPad.TabStop = true;
            this.radioButton_pinPad.Text = "PIN Pad";
            this.radioButton_pinPad.UseVisualStyleBackColor = false;
            // 
            // radioButton_captcha
            // 
            this.radioButton_captcha.AutoSize = true;
            this.radioButton_captcha.BackColor = System.Drawing.Color.White;
            this.radioButton_captcha.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(192)))), ((int)(((byte)(255)))));
            this.radioButton_captcha.FlatAppearance.BorderSize = 2;
            this.radioButton_captcha.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(177)));
            this.radioButton_captcha.ForeColor = System.Drawing.Color.MidnightBlue;
            this.radioButton_captcha.Location = new System.Drawing.Point(19, 399);
            this.radioButton_captcha.Name = "radioButton_captcha";
            this.radioButton_captcha.Size = new System.Drawing.Size(75, 17);
            this.radioButton_captcha.TabIndex = 2;
            this.radioButton_captcha.Text = "CAPTCHA";
            this.radioButton_captcha.UseVisualStyleBackColor = false;
            // 
            // radioButton_okButton
            // 
            this.radioButton_okButton.AutoSize = true;
            this.radioButton_okButton.BackColor = System.Drawing.Color.White;
            this.radioButton_okButton.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(192)))), ((int)(((byte)(255)))));
            this.radioButton_okButton.FlatAppearance.BorderSize = 2;
            this.radioButton_okButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(177)));
            this.radioButton_okButton.ForeColor = System.Drawing.Color.MidnightBlue;
            this.radioButton_okButton.Location = new System.Drawing.Point(19, 373);
            this.radioButton_okButton.Name = "radioButton_okButton";
            this.radioButton_okButton.Size = new System.Drawing.Size(74, 17);
            this.radioButton_okButton.TabIndex = 3;
            this.radioButton_okButton.Text = "OK Button";
            this.radioButton_okButton.UseVisualStyleBackColor = false;
            // 
            // button_initAndRender
            // 
            this.button_initAndRender.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(177)));
            this.button_initAndRender.ForeColor = System.Drawing.Color.MidnightBlue;
            this.button_initAndRender.Location = new System.Drawing.Point(149, 369);
            this.button_initAndRender.Name = "button_initAndRender";
            this.button_initAndRender.Size = new System.Drawing.Size(181, 29);
            this.button_initAndRender.TabIndex = 4;
            this.button_initAndRender.Text = "Initialize and render WYS";
            this.button_initAndRender.UseVisualStyleBackColor = true;
            this.button_initAndRender.Click += new System.EventHandler(this.button_initAndRender_Click);
            // 
            // label_captchaInput
            // 
            this.label_captchaInput.AutoSize = true;
            this.label_captchaInput.ForeColor = System.Drawing.Color.MidnightBlue;
            this.label_captchaInput.Location = new System.Drawing.Point(16, 423);
            this.label_captchaInput.Name = "label_captchaInput";
            this.label_captchaInput.Size = new System.Drawing.Size(109, 13);
            this.label_captchaInput.TabIndex = 5;
            this.label_captchaInput.Text = "CAPTCHA user input:";
            this.label_captchaInput.Visible = false;
            // 
            // textBox_captchaInput
            // 
            this.textBox_captchaInput.ForeColor = System.Drawing.Color.MidnightBlue;
            this.textBox_captchaInput.Location = new System.Drawing.Point(153, 417);
            this.textBox_captchaInput.Name = "textBox_captchaInput";
            this.textBox_captchaInput.Size = new System.Drawing.Size(177, 20);
            this.textBox_captchaInput.TabIndex = 6;
            this.textBox_captchaInput.Visible = false;
            // 
            // button_submit
            // 
            this.button_submit.BackColor = System.Drawing.Color.Gainsboro;
            this.button_submit.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(177)));
            this.button_submit.ForeColor = System.Drawing.Color.MidnightBlue;
            this.button_submit.Location = new System.Drawing.Point(15, 460);
            this.button_submit.Name = "button_submit";
            this.button_submit.Size = new System.Drawing.Size(140, 32);
            this.button_submit.TabIndex = 7;
            this.button_submit.Text = "Submit";
            this.button_submit.UseVisualStyleBackColor = false;
            this.button_submit.Click += new System.EventHandler(this.button_submit_Click);
            // 
            // button_clear
            // 
            this.button_clear.BackColor = System.Drawing.Color.Gainsboro;
            this.button_clear.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(177)));
            this.button_clear.ForeColor = System.Drawing.Color.MidnightBlue;
            this.button_clear.Location = new System.Drawing.Point(188, 460);
            this.button_clear.Name = "button_clear";
            this.button_clear.Size = new System.Drawing.Size(143, 32);
            this.button_clear.TabIndex = 8;
            this.button_clear.Text = "Clear Click Record";
            this.button_clear.UseVisualStyleBackColor = true;
            this.button_clear.Click += new System.EventHandler(this.button_clear_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackgroundImage = global::WysHost.Properties.Resources.WYS;
            this.pictureBox1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.pictureBox1.Location = new System.Drawing.Point(-7, 0);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(361, 60);
            this.pictureBox1.TabIndex = 25;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.BackgroundImage = global::WysHost.Properties.Resources.Background;
            this.pictureBox2.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.pictureBox2.Location = new System.Drawing.Point(-5, 453);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(391, 52);
            this.pictureBox2.TabIndex = 24;
            this.pictureBox2.TabStop = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(348, 499);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.button_clear);
            this.Controls.Add(this.button_submit);
            this.Controls.Add(this.textBox_captchaInput);
            this.Controls.Add(this.label_captchaInput);
            this.Controls.Add(this.button_initAndRender);
            this.Controls.Add(this.radioButton_okButton);
            this.Controls.Add(this.radioButton_captcha);
            this.Controls.Add(this.radioButton_pinPad);
            this.Controls.Add(this.panel_wysView);
            this.Controls.Add(this.pictureBox2);
            this.ForeColor = System.Drawing.Color.MidnightBlue;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Form1";
            this.Text = "WYS Sample";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panel_wysView;
        private System.Windows.Forms.RadioButton radioButton_pinPad;
        private System.Windows.Forms.RadioButton radioButton_captcha;
        private System.Windows.Forms.RadioButton radioButton_okButton;
        private System.Windows.Forms.Button button_initAndRender;
        private System.Windows.Forms.Label label_captchaInput;
        private System.Windows.Forms.TextBox textBox_captchaInput;
        private System.Windows.Forms.Button button_submit;
        private System.Windows.Forms.Button button_clear;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.PictureBox pictureBox1;
    }
}
