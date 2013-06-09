UserPassword
============

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

.. note::

    Symfony 2.2 から ``Symfony\\Component\\Security\\Core\\Validator\\Constraint`` 名前空間の ``UserPassword*`` クラス群は非推奨となり、Symfony 2.3 では削除されています。代わりに ``Symfony\\Component\\Security\\Core\\Validator\\Constraints`` 名前空間の ``UserPassword*`` クラス群を使うようにしてください。

入力値が、現在の認証ユーザーのパスワードと等しいことを検証します。ユーザーのパスワード変更フォームで、セキュリティのために現在のパスワードを入力する必要がある場合に、この制約が役立ちます。

.. note::

    この制約は、ログインフォームでの検証に使ってはいけません。この処理はセキュリティシステムで自動的に処理されるためです。

+----------------+--------------------------------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`                                |
+----------------+--------------------------------------------------------------------------------------------+
| オプション     | - `message`_                                                                               |
+----------------+--------------------------------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Security\\Core\\Validator\\Constraints\\UserPassword`          |
+----------------+--------------------------------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Security\\Core\\Validator\\Constraints\\UserPasswordValidator` |
+----------------+--------------------------------------------------------------------------------------------+

基本的な使い方
--------------

ユーザーが古いパスワードと新しいパスワードを入力してパスワードを変更するフォームで使う ``ChangePassword`` クラスがあるとします。この制約を使うと、古いパスワードが、認証された現在のユーザーのパスワードと一致することを検証できます。

.. configuration-block::

    .. code-block:: yaml

        # src/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Form\Model\ChangePassword:
            properties:
                oldPassword:
                    - Symfony\Component\Security\Core\Validator\Constraints\UserPassword:
                        message: "Wrong value for your current password"

    .. code-block:: php-annotations

        // src/Acme/UserBundle/Form/Model/ChangePassword.php
        namespace Acme\UserBundle\Form\Model;

        use Symfony\Component\Security\Core\Validator\Constraints as SecurityAssert;

        class ChangePassword
        {
            /**
             * @SecurityAssert\UserPassword(
             *     message = "Wrong value for your current password"
             * )
             */
             protected $oldPassword;
        }

    .. code-block:: xml

        <!-- src/UserBundle/Resources/config/validation.xml -->
        <class name="Acme\UserBundle\Form\Model\ChangePassword">
            <property name="Symfony\Component\Security\Core\Validator\Constraints\UserPassword">
                <option name="message">Wrong value for your current password</option>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/UserBundle/Form/Model/ChangePassword.php
        namespace Acme\UserBundle\Form\Model;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Security\Core\Validator\Constraints as SecurityAssert;

        class ChangePassword
        {
            public static function loadValidatorData(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('oldPassword', new SecurityAssert\UserPassword(array(
                    'message' => 'Wrong value for your current password',
                )));
            }
        }

オプション
----------

message
~~~~~~~

**型**: ``message`` **デフォルト**: ``This value should be the user current password``

検証対象の文字列が、現在の認証ユーザーのパスワードと一致しない場合に、このメッセージが表示されます。

.. 2013/06/09 hidenorigoto 80a02647a7f66927edc3bf3401ce657a742f9a1a
