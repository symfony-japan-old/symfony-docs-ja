AssertTrue
==========

.. Validates that a value is ``true``.

値が ``true`` であることのバリデーションを実行します。

.. code-block:: yaml

    properties:
        termsAccepted:
            - AssertTrue: ~

オプション
-------

.. * ``message``: The error message if validation fails

* ``message``: バリデーションが失敗した時のエラーメッセージ。

例
-------

.. This constraint is very useful to execute custom validation logic. You can
   put the logic in a method which returns either ``true`` or ``false``.

この制約はカスタムバリデーションのロジックを実行する際に非常に役立ちます。 ``true`` または ``false`` を返すメソッドにロジックを入れることができます。

.. code-block:: php

    // Sensio/HelloBundle/Author.php
    class Author
    {
        protected $token;

        public function isTokenValid()
        {
            return $this->token == $this->generateToken();
        }
    }

.. Then you can constrain this method with ``AssertTrue``.

``AssertTrue`` でこのメソッドに制約をかけることができます。

.. configuration-block::

    .. code-block:: yaml

        # Sensio/HelloBundle/Resources/config/validation.yml
        Sensio\HelloBundle\Author:
            getters:
                tokenValid:
                    - AssertTrue: { message: "トークンは無効です" }

    .. code-block:: xml

        <!-- Sensio/HelloBundle/Resources/config/validation.xml -->
        <class name="Sensio\HelloBundle\Author">
            <getter name="tokenValid">
                <constraint name="True">
                    <option name="message">トークンは無効です</option>
                </constraint>
            </getter>
        </class>

    .. code-block:: php-annotations

        // Sensio/HelloBundle/Author.php
        class Author
        {
            protected $token;

            /**
             * @validation:AssertTrue(message = "トークンは無効です")
             */
            public function isTokenValid()
            {
                return $this->token == $this->generateToken();
            }
        }

    .. code-block:: php

        // Sensio/HelloBundle/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\AssertTrue;
        
        class Author
        {
            protected $token;
            
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addGetterConstraint('tokenValid', new AssertTrue(array(
                    'message' => 'トークンは無効です',
                )));
            }

            public function isTokenValid()
            {
                return $this->token == $this->generateToken();
            }
        }

.. If the validation of this method fails, you will see a message similar to
   this:

このメソッドのバリデーションが失敗した場合、以下のようなメッセージが表示されます。

.. code-block:: text

    Sensio\HelloBundle\Author.tokenValid:
        This value should not be null
