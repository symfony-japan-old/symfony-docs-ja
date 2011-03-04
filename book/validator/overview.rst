バリデーション
==============

.. Validation is a very common task in web applications. Data entered in forms
   needs to be validated. Data also needs to be validated before it is written
   into a database or passed to a web service.

バリデーションは Web アプリケーションでは非常に一般的なタスクです。フォームに入力されたデータはバリデーションを通す必要があります。また、データベースへ保存したり他の Web サービスに渡したりする前にもバリデーションを行う必要があります。

.. Symfony2 ships with a Validator component that makes this task very easy. This
   component is based on the `JSR303 Bean Validation specification`_. What? A
   Java specification in PHP? You heard right, but it's not as bad as it sounds.
   Let's look at how we use it in PHP.

Symfony2 は、バリデーションのタスクを簡単にするための Validator コンポーネントを備えています。このコンポーネントは `JSR303 Bean Validation specification`_ が元になっています。 `JSR303 Bean Validation specification`_ って何？ PHP で書いた Java の仕様？ それも間違いではありません。しかし、聞くほど悪いものではありません。これから、それを PHP でどのように使うのかを見ていきましょう。

.. The validator validates objects against :doc:`constraints <constraints>`.
   Let's start with the simple constraint that the ``$name`` property of a class
   ``Author`` must not be empty::

バリデータは :doc:`制約 <constraints>` に対してオブジェクトのバリデーションを行います。 ``Author`` クラスのプロパティである ``$name`` が空ではないことを要求する簡単な制約から始めましょう。

.. code-block:: php

    // Sensio/HelloBundle/Author.php
    class Author
    {
        private $name;
    }

.. The next listing shows the configuration that connects properties of the class
   with constraints; this process is called the "mapping":

次の例は、クラスのプロパティと制約を関連付ける設定を表しています。このプロセスを、「マッピング」と呼びます。

.. configuration-block::

    .. code-block:: yaml

        # Sensio/HelloBundle/Resources/config/validation.yml
        Sensio\HelloBundle\Author:
            properties:
                name:
                    - NotBlank: ~

    .. code-block:: xml

        <!-- Sensio/HelloBundle/Resources/config/validation.xml -->
        <class name="Sensio\HelloBundle\Author">
            <property name="name">
                <constraint name="NotBlank" />
            </property>
        </class>

    .. code-block:: php-annotations

        // Sensio/HelloBundle/Author.php
        class Author
        {
            /**
             * @validation:NotBlank()
             */
            private $name;
        }

    .. code-block:: php

        // Sensio/HelloBundle/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Components\Validator\Constraints\NotBlank;

        class Author
        {
            private $name;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('name', new NotBlank());
            }
        }

.. Finally, we can use the :class:`Symfony\\Component\\Validator\\Validator`
   class for :doc:`validation <validation>`. To use the default Symfony2
   validator, adapt your application configuration as follows:

ここでようやく :doc:`validation <validation>` の :class:`Symfony\\Component\\Validator\\Validator` クラスを使用できます。 Symfony2 のデフォルトのバリデータを使用するには、アプリケーションの設定を以下のように行う必要があります。

.. code-block:: yaml

    # hello/config/config.yml
    framework:
        validation:
            enabled: true

.. Now call the ``validate()`` method on the service, which delivers a list of
.. errors if validation fails.

バリデーションに失敗したときにエラーのリストを送信するため、ここでサービス上で ``validate()`` メソッドを呼び出してください。

.. code-block:: php

    $validator = $container->get('validator');
    $author = new Author();

    print $validator->validate($author);

.. Because the ``$name`` property is empty, you will see the following error
   message:

``$name`` プロパティが空なので、以下のエラーメッセージが表示されます。

.. code-block:: text

    Sensio\HelloBundle\Author.name:
        This value should not be blank

.. Insert a value into the property and the error message will disappear.

プロパティに値を入れると、エラーメッセージは消えます。

.. _JSR303 Bean Validation specification: http://jcp.org/en/jsr/detail?id=303
