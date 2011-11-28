フォームログインをカスタマイズする方法
================================

ユーザ認証に、フォームログイン :ref:`form login<book-security-form-login>` を使用することは、 Symfony2 で認証する一般的で柔軟な方法です。そして、フォームログインのほぼ全ての機能をカスタマイズすることができます。次のセクションで全てのデフォルトのコンフィギュレーションを見ていきます。

フォームログインコンフィギュレーションに関して
----------------------------------

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    form_login:
                        # ログインが必要な際に、ここにリダイレクトされます
                        login_path:                     /login

                        # true であれば、リダイレクトする代わりにログインフォームへフォワードします
                        use_forward:                    false

                        # ログインフォームの送信先はここになります
                        check_path:                     /login_check

                        # デフォルトでは、ログインフォームは POST である *必要* があります。 GET ではありません。
                        post_only:                      true

                        # ログイン成功後のリダイレクトオプション。詳細は以下を読んでください。
                        always_use_default_target_path: false
                        default_target_path:            /
                        target_path_parameter:          _target_path
                        use_referer:                    false

                        # ログイン失敗後のリダイレクトオプション。詳細は以下を読んでください。
                        failure_path:                   null
                        failure_forward:                false

                        # ユーザ名とパスワードフィールドの名前
                        username_parameter:             _username
                        password_parameter:             _password

                        # csrf トークンオプション
                        csrf_parameter:                 _csrf_token
                        intention:                      authenticate

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <form-login
                    check_path="/login_check"
                    login_path="/login"
                    use_forward="false"
                    always_use_default_target_path="false"
                    default_target_path="/"
                    target_path_parameter="_target_path"
                    use_referer="false"
                    failure_path="null"
                    failure_forward="false"
                    username_parameter="_username"
                    password_parameter="_password"
                    csrf_parameter="_csrf_token"
                    intention="authenticate"
                    post_only="true"
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('form_login' => array(
                    'check_path'                     => '/login_check',
                    'login_path'                     => '/login',
                    'user_forward'                   => false,
                    'always_use_default_target_path' => false,
                    'default_target_path'            => '/',
                    'target_path_parameter'          => _target_path,
                    'use_referer'                    => false,
                    'failure_path'                   => null,
                    'failure_forward'                => false,
                    'username_parameter'             => '_username',
                    'password_parameter'             => '_password',
                    'csrf_parameter'                 => '_csrf_token',
                    'intention'                      => 'authenticate',
                    'post_only'                      => true,
                )),
            ),
        ));

認証成功後のリダイレクト
-------------------------

ログイン成功後に、コンフィギュレーションオプションを指定することで、リダイレクト先を変更することができます。デフォルトでは、フォームはユーザがリクエストした URL にリダイレクトします(ログインフォームを表示することになった URL)。例えば、 ``http://www.example.com/admin/post/18/edit`` にリクエストにしたのであれば、ログイン成功後に、同じ URL の ``http://www.example.com/admin/post/18/edit`` にリダイレクトされます。これは、最初にリクエストされた URL をセッションに格納することで実現しています。直接ログインページにアクセスしたときなど、セッションに URL が無い際には、デフォルトのページにリダイレクトされます。デフォルトでは、 ``/`` になっています。いろんな方法で、このリダイレクトに関する振る舞いを変更することができます。

.. note::

    説明しましたように、デフォルトでは、ユーザは最初にリクエストしたページにリダイレクトされ、戻ることになります。しかし、これが問題となるときもあります。例えばバックグラウンドの AJAX のリクエストが最後に訪れた URL となっており、ユーザがその URL にリダイレクトされてしまうときです。この振る舞いを制御する詳細は、 :doc:`/cookbook/security/target_path` を参照してください。

デフォルトページを変更する
~~~~~~~~~~~~~~~~~~~~~~~~~

デフォルトページの変更は可能です。デフォルトページとは、最初にアクセスしたページの URL をセッションに格納していなかった際にリダイレクトされるページです。デフォルトページに ``/admin`` をセットするには、次のコンフィギュレーションを使用します。

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    form_login:
                        # ...
                        default_target_path: /admin

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <form-login
                    default_target_path="/admin"                    
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('form_login' => array(
                    // ...
                    'default_target_path' => '/admin',
                )),
            ),
        ));


これでセッションに URL がセットされていなければ、 ``/admin`` にリダイレクトされるようになりました。

常にデフォルトページにリダイレクトする
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

以前にリクエストした URL を持っていようが、常にデフォルトページにリダイレクトすることもできます。 ``always_use_default_target_path`` オプションを ``true`` にセットして実現できます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    form_login:
                        # ...
                        always_use_default_target_path: true
                        
    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <form-login
                    always_use_default_target_path="true"
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('form_login' => array(
                    // ...
                    'always_use_default_target_path' => true,
                )),
            ),
        ));

リファラー URL を使用する
~~~~~~~~~~~~~~~~~~~~~~~

以前の URL がセッションに格納されていなかった際に、 ``HTTP_REFERER`` を代わりに使用することができます。これを実現するには、 ``use_referer`` を ``true`` にしてください。デフォルトでは、 ``false`` になっています。

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    form_login:
                        # ...
                        use_referer:        true

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <form-login
                    use_referer="true"
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('form_login' => array(
                    // ...
                    'use_referer' => true,
                )),
            ),
        ));

.. versionadded:: 2.1
    Symfony 2.1 では、リファラーの値が ``login_path`` オプションと同じの際には、ユーザは ``default_target_path`` にリダイレクトされます。

フォーム内でリダイレクト先 URL を制御する
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

名前が ``_target_path`` の hidden フィールドを加えることで、フォーム自体にリダイレクト先を指定させることもできます。例えば、 ``account`` というルートに定義された URL にリダイレクトするには、次のように指定してください。

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

            <input type="hidden" name="_target_path" value="account" />

            <input type="submit" name="login" />
        </form>

    .. code-block:: html+php

        <?php // src/Acme/SecurityBundle/Resources/views/Security/login.html.php ?>
        <?php if ($error): ?>
            <div><?php echo $error->getMessage() ?></div>
        <?php endif; ?>

        <form action="<?php echo $view['router']->generate('login_check') ?>" method="post">
            <label for="username">Username:</label>
            <input type="text" id="username" name="_username" value="<?php echo $last_username ?>" />

            <label for="password">Password:</label>
            <input type="password" id="password" name="_password" />

            <input type="hidden" name="_target_path" value="account" />
            
            <input type="submit" name="login" />
        </form>

これで、 hidden フォームフィールドの値へリダイレクトされるようになりました。この値は、相対パス、絶対パス、ルート名を指定することができます。また、 ``target_path_parameter`` オプションで他の値に変更すれば、 hidden フォームフィールドの名前も変更することができます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    form_login:
                        target_path_parameter: redirect_url

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <form-login
                    target_path_parameter="redirect_url"
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('form_login' => array(
                    'target_path_parameter' => redirect_url,
                )),
            ),
        ));

ログイン失敗時のリダイレクト
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ログイン成功後のリダイレクトだけでなく、ログイン失敗後(ユーザ名、パスワードが間違っていた際など)のリダイレクトも指定することができます。デフォルトでは、ログインフォーム自体にリダイレクトされますが、他の URL に指定することもできます。次のコンフィギュレーションを見てください。

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    form_login:
                        # ...
                        failure_path: /login_failure
                        
    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <form-login
                    failure_path="login_failure"
                />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('form_login' => array(
                    // ...
                    'failure_path' => login_failure,
                )),
            ),
        ));


.. 2011/11/28 ganchiku 8ca9b8576f45f3ada90e33de25160f782c061bed

