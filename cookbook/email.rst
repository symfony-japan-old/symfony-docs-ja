.. index::
   single: Emails

メールの送信方法
====================

メール送信は、ウェブアプリケーションを作る際のよくある困難なタスクです。機能を実装するのに、車輪の再発明をするのではなく、 `SwiftMailer`_ ライブラリを活用した ``SwiftmailerBundle`` を使用してメール送信機能を実装することができます。

.. note::

   使用する前に、 ``SwiftmailerBundle`` バンドルを使用可能にしていることを確認してください。
   ::

        public function registerBundles()
        {
            $bundles = array(
                // ...
                new Symfony\Bundle\SwiftmailerBundle\SwiftmailerBundle(),
            );

            // ...
        }

.. _swift-mailer-configuration:

コンフィギュレーション
-------------

Swiftmailer を使用する前に、コンフィギュレーションをチェックしましょう。指定が必要なパラメターは、 ``transport`` だけです。
:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        swiftmailer:
            transport:  smtp
            encryption: ssl
            auth_mode:  login
            host:       smtp.gmail.com
            username:   your_username
            password:   your_password

    .. code-block:: xml

        <!-- app/config/config.xml -->

        <!--
        xmlns:swiftmailer="http://symfony.com/schema/dic/swiftmailer"
        http://symfony.com/schema/dic/swiftmailer http://symfony.com/schema/dic/swiftmailer/swiftmailer-1.0.xsd
        -->

        <swiftmailer:config
            transport="smtp"
            encryption="ssl"
            auth-mode="login"
            host="smtp.gmail.com"
            username="your_username"
            password="your_password" />

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('swiftmailer', array(
            'transport'  => "smtp",
            'encryption' => "ssl",
            'auth_mode'  => "login",
            'host'       => "smtp.gmail.com",
            'username'   => "your_username",
            'password'   => "your_password",
        ));

Swiftmailer コンフィギュレーションのほとんどは、メッセージの配送方法に関するものです。

次のコンフィギュレーションを指定することができます。
:

* ``transport``         (``smtp``, ``mail``, ``sendmail``, or ``gmail``)
* ``username``
* ``password``
* ``host``
* ``port``
* ``encryption``        (``tls``, or ``ssl``)
* ``auth_mode``         (``plain``, ``login``, or ``cram-md5``)
* ``spool``

  * ``type`` (現在は ``file`` のみを指定えきますが、どうやってキューを行うかを指定します)
  * ``path`` (メッセージを格納する場所)
* ``delivery_address``  (全てのメールを送るメールアドレス)
* ``disable_delivery``  (配送の失敗を許容するには true を指定してください)

メールの送信
--------------

Swiftmailer ライブラリは、 ``Swift_Message`` オブジェクトを作成し、設定し、そして送信することで動作をします。 ``mailer`` は実際のメッセージの配送に対して責任を持って、 ``mailer`` サービスを介してアクセスが可能です。結果、メールの送信は素直な実装になります。
::

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

疎な結合をするために、 メールの本文は、テンプレートに格納され、 ``renderView()`` メソッドで内容を作成しています。

``$message`` オブジェクトは、添付ファイルや、 HTML コンテントを追加したりするなど、さらに多くのオプションをサポートします。幸運なことに Swiftmailer のドキュメントは充実しており、 `Creating Message` の章を参照して、詳細を調べることができます。

.. tip::

    また、 Symfony2 でのメール送信に関する情報は、他のクックブックの記事も参考になります。
    :

    * :doc:`gmail`
    * :doc:`email/dev_environment`
    * :doc:`email/spool`

.. _`Swiftmailer`: http://www.swiftmailer.org/
.. _`Creating Messages`: http://swiftmailer.org/docs/messages

.. 2011/10/31 ganchiku 99eb43fff504594b2cea44a137939140620d26c4

