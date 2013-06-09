True
====

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/7

値が ``true`` であることを検証します。
つまり、値が厳密に ``true``\ 、または整数値の ``1``\ 、または文字列の "``1``" であるかどうかをチェックします。

:doc:`False <False>` も参照してください。

+----------------+---------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`         |
+----------------+---------------------------------------------------------------------+
| オプション     | - `message`_                                                        |
+----------------+---------------------------------------------------------------------+
| Class          | :class:`Symfony\\Component\\Validator\\Constraints\\True`           |
+----------------+---------------------------------------------------------------------+
| Validator      | :class:`Symfony\\Component\\Validator\\Constraints\\TrueValidator`  |
+----------------+---------------------------------------------------------------------+

基本的な使い方
--------------

この制約はプロパティ(たとえば登録モデルの ``termsAccepted`` プロパティ)および "ゲッター" メソッドに適用できます。
ゲッターメソッドの戻り値が true であることを確かめる場合に威力を発揮します。
たとえば、次のようなメソッドがあるとします。

.. code-block:: php

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    class Author
    {
        protected $token;

        public function isTokenValid()
        {
            return $this->token == $this->generateToken();
        }
    }

isTokenValid() メソッドの戻り値が ``True`` であることを保証するには、次のように設定します。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            getters:
                tokenValid:
                    - "True": { message: "The token is invalid." }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            protected $token;

            /**
             * @Assert\True(message = "The token is invalid")
             */
            public function isTokenValid()
            {
                return $this->token == $this->generateToken();
            }
        }

    .. code-block:: xml

        <!-- src/Acme/Blogbundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <getter property="tokenValid">
                <constraint name="True">
                    <option name="message">The token is invalid.</option>
                </constraint>
            </getter>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\True;
        
        class Author
        {
            protected $token;
            
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addGetterConstraint('tokenValid', new True(array(
                    'message' => 'The token is invalid.',
                )));
            }

            public function isTokenValid()
            {
                return $this->token == $this->generateToken();
            }
        }

``isTokenValid()`` の戻り値が false だった場合、バリデーションに失敗します。

オプション
----------

message
~~~~~~~

**タイプ**: ``string`` **デフォルト**: ``This value should be true``

検証するデータが true ではなかった場合にこのメッセージが表示されます。

.. 2012/01/31 hidenorigoto 3097cd214cf6a4a8090004e946dcc85202821710
.. 2013/06/09 hidenorigoto b7b28c17c446ab8808eaaf48b3c14e4db97fad65
