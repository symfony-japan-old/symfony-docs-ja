.. index::
   single: Emails; Gmail

Gmail を使用してメールを送信する方法
====================================

開発環境では、 SMTP サーバを使用してメールを送信するよりも、 Gmail を使用した方が簡単で実践的です。 Swfitmailer バンドルを使用すれば、Gmail を使用したメール送信を簡単に実現できます。

.. tip::

    普段使用している Gmail のアカウントをそのまま使用するのではなく、特別なアカウントを作成することをお勧めします。 

次のように、開発環境のコンフィギュレーションファイルの、 ``transport`` の設定を ``gmail`` に変更して、 ``username`` と ``password`` をそのアカウント用のものに指定してください。
:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        swiftmailer:
            transport: gmail
            username:  your_gmail_username
            password:  your_gmail_password

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            transport="gmail"
            username="your_gmail_username"
            password="your_gmail_password" />

    .. code-block:: php

        // app/config/config_dev.php
        $container->loadFromExtension('swiftmailer', array(
            'transport' => "gmail",
            'username'  => "your_gmail_username",
            'password'  => "your_gmail_password",
        ));

これだけです！

.. note::

    ``gmail`` トランスポートは、単なるショートカットで、実際は、 ``smtp`` トランスポートを使用し、 ``encription`` 、 ``auth_mode`` 、 ``host`` を Gmail 用にしたものになります。

.. 2011/10/31 ganchiku c27197749719c196db039a93ecb06f391272cd61

