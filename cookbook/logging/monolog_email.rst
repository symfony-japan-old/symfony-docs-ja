エラーメールを送る Monolog の設定方法
========================================

Monolog_ は、アプリケーションでエラーが起きた際にメールを送信するように設定することができます。あまりにも多くのメールを受け取らないようにするために、コンフィギュレーションで少しネストされたハンドラが必要になります。次のようになりますが、最初は複雑に見えますがハンドラ１つずつを見ていけば、とても簡単です。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        monolog:
            handlers:
                mail:
                    type:         fingers_crossed
                    action_level: critical
                    handler:      buffered
                buffered:
                    type:    buffer
                    handler: swift
                swift:
                    type:       swift_mailer
                    from_email: error@example.com
                    to_email:   error@example.com
                    subject:    An Error Occurred!
                    level:      debug

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/monolog http://symfony.com/schema/dic/monolog/monolog-1.0.xsd">

            <monolog:config>
                <monolog:handler
                    name="mail"
                    type="fingers_crossed"
                    action-level="critical"
                    handler="buffered"
                />
                <monolog:handler
                    name="buffered"
                    type="buffer"
                    handler="swift"
                />
                <monolog:handler
                    name="swift"
                    from-email="error@example.com"
                    to-email="error@example.com"
                    subject="An Error Occurred!"
                    level="debug"
                />
            </monolog:config>
        </container>

``mail`` ハンドラは ``fingers_crossed`` ハンドラで、これはつまり指定したアクションレベルに到達したときのみトリガーされます。上記の場合であれば、 ``critical`` の際に到達したとみなされます。そうなると ``critical`` より下のアクションレベルのメッセージも全てログをとるようになります。 ``critical`` レベルは HTTP のエラーステータスの 500 番台のときのみトリガーされます。 ``handler`` 設定は、出力が ``buffered`` ハンドラに渡されることを意味しています。

.. tip::

    もし 400 番台と 500 番台のエラーレベルでメール送信をトリガーしたい際には、 ``action_level`` を ``critical`` ではなく ``error`` に設定してください。

``buffered`` ハンドラは、リクエストの全てのメッセージを保持して、ネストされたハンドラにそのメッセージを渡すだけです。このハンドラを使用しなければ、それぞれのメッセージは別々にメール送信されてしまいます。このハンドラを使うことで ``swift`` ハンドラにメッセージが渡されます。 ``swift`` あは、実際にエラーメールの送信を担います。この設定は送信先のアドレス、送信者のアドレス、そして件名のみで、簡単です。

これらのハンドラは、他のハンドラと組み合わせることができますので、メールを送信するのと同時にサーバにもログを付けることができます。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        monolog:
            handlers:
                main:
                    type:         fingers_crossed
                    action_level: critical
                    handler:      grouped
                grouped:
                    type:    group
                    members: [streamed, buffered]
                streamed:
                    type:  stream
                    path:  %kernel.logs_dir%/%kernel.environment%.log
                    level: debug
                buffered:
                    type:    buffer
                    handler: swift
                swift:
                    type:       swift_mailer
                    from_email: error@example.com
                    to_email:   error@example.com
                    subject:    An Error Occurred!
                    level:      debug

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:monolog="http://symfony.com/schema/dic/monolog"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/monolog http://symfony.com/schema/dic/monolog/monolog-1.0.xsd">

            <monolog:config>
                <monolog:handler
                    name="main"
                    type="fingers_crossed"
                    action_level="critical"
                    handler="grouped"
                />                
                <monolog:handler
                    name="grouped"
                    type="group"
                >
                    <member type="stream"/>
                    <member type="buffered"/>
                </monolog:handler>
                <monolog:handler
                    name="stream"
                    path="%kernel.logs_dir%/%kernel.environment%.log"
                    level="debug"
                />
                <monolog:handler
                    name="buffered"
                    type="buffer"
                    handler="swift"
                />
                <monolog:handler
                    name="swift"
                    from-email="error@example.com"
                    to-email="error@example.com"
                    subject="An Error Occurred!"
                    level="debug"
                />
            </monolog:config>
        </container>

この設定では ``group`` ハンドラを使って ``buffered`` ハンドラと ``stream`` ハンドラの２つのグループメンバーにメッセージを送っています。これでメッセージはログファイルに書かれますし、またメールも送信されるようになりました。

.. _Monolog: https://github.com/Seldaek/monolog

.. 2011/12/27 ganchiku 9ffb64717371401e9387f2499101b1628ce62c05

