制約のバリデーション
====================

.. Objects with constraints are validated by the
   :class:`Symfony\\Component\\Validator\\Validator` class. If you use Symfony2,
   this class is already registered as a service in the Dependency Injection
   Container. To enable the service, add the following lines to your
   configuration:

制約付きのオブジェクトは、 :class:`Symfony\\Component\\Validator\\Validator` によってバリデーションが行われます。 Symfony2 を使用している場合、このクラスは DI コンテナ内であらかじめサービスとして登録されています。サービスを使用可能にするには、以下の行を設定に追加してください。

.. code-block:: yaml

    # hello/config/config.yml
    framework:
        validation:
            enabled: true

.. Then you can get the validator from the container and start validating your
   objects:

これでコンテナからバリデータを使用可能にし、オブジェクトのバリデーションを開始できます。

.. code-block:: php

    $validator = $container->get('validator');
    $author = new Author();

    print $validator->validate($author);

.. The ``validate()`` method returns a
   :class:`Symfony\\Component\\Validator\\ConstraintViolationList` object. This
   object behaves exactly like an array. You can iterate over it and you can even
   print it in a nicely formatted manner. Every element of the list corresponds
   to one validation error. If the list is empty, it's time to dance, because
   then validation succeeded.

``validate()`` メソッドは、 :class:`Symfony\\Component\\Validator\\ConstraintViolationList` クラスを返します。このオブジェクトはまさしく配列のように振舞います。イテレートすることもできますし、きれいに整形されたかたちで表示することもできてしまいます。リストのそれぞれの要素は、1つのバリデーションのエラーに対応します。リストが空の時は、ダンスしましょう。バリデーションが成功したということですから。

.. The above call will output something similar to this:

上に挙げた呼び出しは、以下と同じような出力を返します。

.. code-block:: text

    Sensio\HelloBundle\Author.firstName:
        This value should not be blank
    Sensio\HelloBundle\Author.lastName:
        This value should not be blank
    Sensio\HelloBundle\Author.fullName:
        This value is too short. It should have 10 characters or more

.. If you fill the object with correct values the validation errors disappear.

オブジェクトに正しい値を入れた場合、バリデーションのエラーは消えます。
