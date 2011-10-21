"Remember Me" ログイン機能の追加方法
============================================

ユーザが認証されると、通常はセッションにその信用証明書(credential)が格納されます。これは、セッションが終了したら、ユーザは、ログアウトされたことになり、次回アプリケーションにアクセスをした際に、再びログインをしなければならないということを意味します。あなたは ``remember_me`` ファイアーウォールオプションのクッキーを使用して、ユーザに対し、通常のセッションよりも長くログイン状態を保つことができる選択肢を与えることができます。このファイアーウォールでは、クッキー情報を暗号化するために使われるシークレットキーを設定する必要があります。また、その他のデフォルト値を持ったオプションの値は次の通りです:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        firewalls:
            main:
                remember_me:
                    key:      aSecretKey
                    lifetime: 3600
                    path:     /
                    domain:   ~ # デフォルトは $_SERVER で取得される現在のドメインです

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <remember-me
                    key="aSecretKey"
                    lifetime="3600"
                    path="/"
                    domain="" <!-- デフォルトは $_SERVER で取得される現在のドメインです -->
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('remember_me' => array(
                    'key'                     => '/login_check',
                    'lifetime'                => 3600,
                    'path'                    => '/',
                    'domain'                  => '', // Defaults to the current domain from $_SERVER
                )),
            ),
        ));

remember me 機能が不適切な際もあるため、その使用に関する選択をユーザに提供するのは、良い考えです。一般的な方法は、ログインフォームにチェックボックスを加えることです。ログインが成功した際に、 ``_remember_me`` という名前のチェックボックスがチェックされていれば、クッキーは自動的にセットされます。この機能を実装したログインフォームは最終的に次のようになります:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/SecurityBundle/Resources/views/Security/login.html.twig #}
        {% if error %}
            <div>{{ error.message }}</div>
        {% endif %}

        <form action="{{ path('login_check') }}" method="post">
            <label for="username">Username:</label>
            <input type="text" id="username" name="_username" value="{{ last_username }}" />

            <label for="password">Password:</label>
            <input type="password" id="password" name="_password" />

            <input type="checkbox" id="remember_me" name="_remember_me" checked />
            <label for="remember_me">Keep me logged in</label>

            <input type="submit" name="login" />
        </form>

    .. code-block:: html+php

        <?php // src/Acme/SecurityBundle/Resources/views/Security/login.html.php ?>
        <?php if ($error): ?>
            <div><?php echo $error->getMessage() ?></div>
        <?php endif; ?>

        <form action="<?php echo $view['router']->generate('login_check') ?>" method="post">
            <label for="username">Username:</label>
            <input type="text" id="username" 
                   name="_username" value="<?php echo $last_username ?>" />

            <label for="password">Password:</label>
            <input type="password" id="password" name="_password" />

            <input type="checkbox" id="remember_me" name="_remember_me" checked />
            <label for="remember_me">Keep me logged in</label>

            <input type="submit" name="login" />
        </form>

クッキーが有効である間、ユーザが次回アクセスすれば、自動的にログインされることになります。

特定のリソースへのアクセスの前にもう一度ユーザ認証をさせる
----------------------------------------------------------------------

あなたのサイトに一度認証されたユーザが戻ってきた際に、そのユーザは remember me クッキーによって自動的にログインされます。このことにより、そのユーザは、サイトで実際に認証されたかのように、保護されたリソースへのアクセスを許可されます。

しかし、特定のリソースへのアクセスさせる前にもう一度、ユーザを実際に認証したいときもあるでしょう。例えば、"remember me" で認証されたユーザに対して、アカウント情報の参照は可能とするが、その内容を変更するためには実際にもう一度認証をしたいときなどです。

セキュリティコンポーネントは、この実現方法を提供します。ユーザには、明示的に割り当てられたロールに加え、認証方法よって、次のロールのいずれかを自動的に与えることができます:

* ``IS_AUTHENTICATED_ANONYMOUSLY`` - 実際にログインしていないユーザが、サイト上のファイアーウォールの保護された場所にアクセスしたユーザに自動的に割り当てるロール。これは匿名アクセスを許しているときのみ使用可能です。

* ``IS_AUTHENTICATED_REMEMBERED`` - remember me クッキーを介して認証されたユーザに自動的に割り当てるロール。

* ``IS_AUTHENTICATED_FULLY`` - 現在のセッション中にログインをしたユーザに自動的に割り当てるロール。

明示的に割り当てられたロールよりも上のレベルで、れらのコントロールアクセスを使用することができます。

.. note::

    ``IS_AUTHENTICATED_REMEMBERED`` ロールを保持していれば、``IS_AUTHENTICATED_ANONYMOUSLY`` ロールも保持していることになります。 ``IS_AUTHENTICATED_FULLY`` ロールを保持していれば、これらの２つのロールを保持していることになります。言い換えると、これらのロールは、認証の "強度" の３つのレベルを表現していることになります。

サイトのアクセスコントロールをより洗練させた緻密にするために、これらの追加のロールを使用することができます。例えば、クッキーによる認証の際には ``/acount`` にアクセスしてアカウント情報を参照できるようにしたいとします。また、その際に同時にアカウント情報を編集するためには、実際のログインが必要にしたいとします。これは、これらのロールを使って特定のコントローラアクションをセキュアにすることで実現することができます。コントローラの編集を行うアクション(edit action)は service context を使用してセキュアにすることができます。

下の例では、 ``IS_AUTHENTICATED_FULLY`` ロールを保持しているユーザのみ、アクションへ受け入れが可能になっています。

.. code-block:: php

    use Symfony\Component\Security\Core\Exception\AccessDeniedException
    // ...

    public function editAction()
    {
        if (false === $this->get('security.context')->isGranted(
            'IS_AUTHENTICATED_FULLY'
        )) {
            throw new AccessDeniedException();
        }

        // ...
    }

また、オプションの JMSSecurityExtraBundle_ をインストールして、アノテーションによるコントローラのセキュア化をすることもできます:

.. code-block:: php

    use JMS\SecurityExtraBundle\Annotation\Secure;

    /**
     * @Secure(roles="IS_AUTHENTICATED_FULLY")
     */
    public function editAction($name)
    {
        // ...
    }

.. tip::

    セキュリティコンフィギュレーションで、アカウントに関する全てのリソースにアクセスするには ``ROLE_USER`` ロールが必須であるというアクセスコントロールをすでに使用している際には、次のシチュエーションがあることを想定してください:
    
    * 認証されていないユーザ、もしくは匿名認証ユーザが、アカウントリソースにくアセスを試みた際に、そのユーザに認証を訪ねます

    * 一度あるユーザが、ユーザ名とパスワードを入力してユーザが認証されて、設定にあるように ``ROLE_USER`` ロールが割り当てられると、そのユーザは ``IS_AUTHENTICATED_FULLY`` ロールを保持することになり、 ``editAction`` コントローラを含むアカウントのセクションにある全てのページにアクセス可能になります。

    * ユーザのセッションが終了すると、そのユーザがサイトに再び訪れた際に、編集ページ以外の全てのアカウントのページにアクセスが可能となります。編集ページは、再認証が必要となり、同ユーザが ``editAction`` コントローラにアクセスを試みた際には、完全なる認証はされていないため、再認証をすることになります。
    

このようなサービスやメソッドをセキュアにする方法の詳細は、 :doc:`/cookbook/security/securing_services` を参照してください。

.. _JMSSecurityExtraBundle: https://github.com/schmittjoh/JMSSecurityExtraBundle
