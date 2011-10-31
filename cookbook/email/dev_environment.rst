開発中におけるメール送信の扱い方
==========================================

メールを送信するアプリケーションを作成している際に、開発中では、実際に受信者にメールを送信したくないときもあると思います。 Symfony2 の ``SwiftmailerBundle`` を使用すれば、コードの変更をすることなしに、コンフィギュレーションの設定を変更するだけで、この問題を簡単に解決することができます。開発中では、メール送信の処理に関して、主に２つの選択をすることができます。: (a) メール送信を無効にする (b) 特定のメールアドレスにメールを送信する

メール送信を無効にする
-----------------

``disable_delivery`` オプションを ``true`` に設定すればメール送信を無効にすることができます。 Symfony の Standard Distribution の ``test`` 環境では、デフォルトでメール送信を無効にしており、この設定になっています。 ``test`` 環境でこのようによう指定しているため、テストを実行する際にメールは送信されません。しかし、 ``prod`` や ``dev`` 環境では、送信されます。
:
.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml
        swiftmailer:
            disable_delivery:  true

    .. code-block:: xml

        <!-- app/config/config_test.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            disable-delivery="true" />

    .. code-block:: php

        // app/config/config_test.php
        $container->loadFromExtension('swiftmailer', array(
            'disable_delivery'  => "true",
        ));

``dev`` 環境でも送信を無効にしたい際には、 ``config_dev.yml`` ファイルに同じ設定を追加するだけです。

特定のメールアドレスに送信する
------------------------------

また、メッセージを送信する際に、実際に指定したメールアドレスではなく、全てのメールの送信先を特定のアドレスにすることもできます。 ``delivery_address`` オプションを使用してこの設定ができます。
:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_dev.yml
        swiftmailer:
            delivery_address:  dev@example.com

    .. code-block:: xml

        <!-- app/config/config_dev.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            delivery-address="dev@example.com" />

    .. code-block:: php

        // app/config/config_dev.php
        $container->loadFromExtension('swiftmailer', array(
            'delivery_address'  => "dev@example.com",
        ));

そして、次のように ``recipient@example.com`` にメールを送信しようとしてみます。

.. code-block:: php

    public function indexAction($name)
    {
        $message = \Swift_Message::newInstance()
            ->setSubject('Hello Email')
            ->setFrom('send@example.com')
            ->setTo('recipient@example.com')
            ->setBody($this->renderView('HelloBundle:Hello:email.txt.twig', array('name' => $name)))
        ;
        $this->get('mailer')->send($message);

        return $this->render(...);
    }

``dev`` 環境では、メールは、 ``recipient@example.com`` には送信されずに ``dev@example.com`` に送信されます。 Swiftmailer は、置き換えられたアドレスを ``X-Swfit-To`` に指定して、メールのヘッダーに追加します。そうすることによって、実際に送信したかったアドレスを参照することができます。

.. note::

    ``to`` に指定したアドレスのみならず、 ``CC`` や ``BCC`` に指定したアドレスにも送信されることはありません。 Swiftmailer はこれらのアドレスを上書きして、 ``CC`` であれば、 ``X-Swift-Cc`` に、 ``BCC`` であれば、 ``X-Swift-BCC`` としてメールヘッダに追加します。

ウェブデバッグツールバーで参照する
----------------------------------

``dev`` 環境を使用していれば、そのページで送信されたメールは、ウェブデバッグツールバーを使って参照することができます。ツールバーのメールアイコンは送信したメールの数を表しています。そのアイコンをクリックすると、メールの詳細のレポートを見ることができます。

メール送信後、すぐにリダイレクトをする場合は、 ``config_dev.yml`` ファイルの ``intercept_redirects`` オプションを ``true`` にすれば、リダイレクトする前のメールをウェブでバッグツールバーで参照することができます。

.. 2011/10/31 ganchiku 3db3df5db2c9150678d1f968683b33bd11809a4b

