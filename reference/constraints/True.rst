.. 2011/07/24 yanchi 06f3bcba3d245cdaf7fc8bc21eb83b03e7258be7

True
====

値が ``true`` あることを検証します。

.. code-block:: yaml

    properties:
        termsAccepted:
            - True: ~

オプション
----------

* ``message``: 検証に失敗した場合のエラーメッセージ

例
--

この制約は、カスタムした検証処理を実行するのにとても便利です。
あなたはメソッドに ``true`` または、 ``false`` を返す処理を記述することが出来ます。

.. code-block:: php

    // Acme/HelloBundle/Entity/Author.php
    class Author
    {
        protected $token;

        public function isTokenValid()
        {
            return $this->token == $this->generateToken();
        }
    }

それからあなたはこのメソッドで ``True`` を制約することが出来ます。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/validation.yml
        Acme\HelloBundle\Author:
            getters:
                tokenValid:
                    - True: { message: "The token is invalid" }

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/validation.xml -->
        <class name="Acme\HelloBundle\Author">
            <getter name="tokenValid">
                <constraint name="True">
                    <option name="message">The token is invalid</option>
                </constraint>
            </getter>
        </class>

    .. code-block:: php-annotations

        // src/Acme/HelloBundle/Author.php
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

    .. code-block:: php

        // src/Acme/HelloBundle/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\True;
        
        class Author
        {
            protected $token;
            
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addGetterConstraint('tokenValid', new True(array(
                    'message' => 'The token is invalid',
                )));
            }

            public function isTokenValid()
            {
                return $this->token == $this->generateToken();
            }
        }

このメソッドの検証が失敗した場合、このようなメッセージが表示されます。:

.. code-block:: text

    Acme\HelloBundle\Author.tokenValid:
        This value should not be null
